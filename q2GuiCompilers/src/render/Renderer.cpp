#include "Core.h"
#include "Renderer.h"
#include "atlas/Atlas.inl"

namespace Q2Compilers
{
	Renderer::Renderer(GLFWwindow* window)
	{
		_window = window;
		glfwGetFramebufferSize(window, &_width, &_height);
		glViewport(0, 0, _width, _height);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glGenTextures(1, &_atlasTex);
		glBindTexture(GL_TEXTURE_2D, _atlasTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ATLAS_WIDTH, ATLAS_HEIGHT, 0,
			GL_ALPHA, GL_UNSIGNED_BYTE, atlas_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		ASSERT(glGetError() == 0, "Failed to initialize the renderer");
	}

	Renderer::~Renderer()
	{
		if (_atlasTex)
		{
			glDeleteTextures(1, &_atlasTex);
		}
	}

	void Renderer::ProcessCommands(mu_Context* context, mu_Color clearColor)
	{
		Clear(clearColor);

		if (!context)
		{
			return;
		}

		mu_Command* cmd = NULL;
		while (mu_next_command(context, &cmd))
		{
			switch (cmd->type)
			{
			case MU_COMMAND_TEXT:
				CmdText(cmd->text.str, cmd->text.pos, cmd->text.color);
				break;
			case MU_COMMAND_RECT:
				CmdRect(cmd->rect.rect, cmd->rect.color);
				break;
			case MU_COMMAND_ICON:
				CmdIcon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
				break;
			case MU_COMMAND_CLIP:
				CmdClip(cmd->clip.rect);
				break;
			default:
				break;
			}
		}
	}

	int Renderer::GetTextWidth(const char* text, int len)
	{
		int res = 0;
		for (const char* p = text; *p && len--; p++)
		{
			if ((*p & 0xc0) == 0x80)
			{
				continue;
			}
			int chr = mu_min((unsigned char)*p, 127);
			res += atlas_map.at(ATLAS_FONT + chr).w;
		}
		return res;
	}

	void Renderer::Finish()
	{
		Flush();
	}

	void Renderer::CmdText(const char* text, mu_Vec2 pos, mu_Color color)
	{
		mu_Rect dst = { pos.x, pos.y, 0, 0 };

		for (const char* p = text; *p; p++)
		{
			if ((*p & 0xc0) == 0x80)
			{
				continue;
			}
			int chr = mu_min((unsigned char)*p, 127);
			ASSERT(atlas_map.find(ATLAS_FONT + chr) != atlas_map.end(), "Atlas: index not found");
			mu_Rect src = atlas_map.at(ATLAS_FONT + chr);
			dst.w = src.w;
			dst.h = src.h;
			PushQuad(dst, src, color);
			dst.x += dst.w;
		}
	}

	void Renderer::CmdRect(mu_Rect rect, mu_Color color)
	{
		PushQuad(rect, atlas_map.at(ATLAS_WHITE), color);
	}

	void Renderer::CmdIcon(int id, mu_Rect rect, mu_Color color)
	{
		mu_Rect src = atlas_map.at(id);
		int x = rect.x + (rect.w - src.w) / 2;
		int y = rect.y + (rect.h - src.h) / 2;
		PushQuad(mu_rect(x, y, src.w, src.h), src, color);
	}

	void Renderer::CmdClip(mu_Rect rect)
	{
		Flush();
		glScissor(rect.x, _height - (rect.y + rect.h), rect.w, rect.h);
	}

	void Renderer::PushQuad(mu_Rect dst, mu_Rect src, mu_Color color)
	{
		if (buf.i == RENDER_BUFFER_SIZE)
		{
			Flush();
		}

		int texvert_idx = buf.i * 8;
		int   color_idx = buf.i * 16;
		int element_idx = buf.i * 4;
		int   index_idx = buf.i * 6;
		buf.i++;

#pragma warning( push )
#pragma warning( disable : 6386 )

		//update texture buffer
		float x = src.x / (float)ATLAS_WIDTH;
		float y = src.y / (float)ATLAS_HEIGHT;
		float w = src.w / (float)ATLAS_WIDTH;
		float h = src.h / (float)ATLAS_HEIGHT;
		buf.texture[texvert_idx + 0] = x;
		buf.texture[texvert_idx + 1] = y;
		buf.texture[texvert_idx + 2] = x + w;
		buf.texture[texvert_idx + 3] = y;
		buf.texture[texvert_idx + 4] = x;
		buf.texture[texvert_idx + 5] = y + h;
		buf.texture[texvert_idx + 6] = x + w;
		buf.texture[texvert_idx + 7] = y + h;

		//update vertex buffer
		buf.vertices[texvert_idx + 0] = (GLfloat)dst.x;
		buf.vertices[texvert_idx + 1] = (GLfloat)dst.y;
		buf.vertices[texvert_idx + 2] = (GLfloat)(dst.x + dst.w);
		buf.vertices[texvert_idx + 3] = (GLfloat)dst.y;
		buf.vertices[texvert_idx + 4] = (GLfloat)dst.x;
		buf.vertices[texvert_idx + 5] = (GLfloat)(dst.y + dst.h);
		buf.vertices[texvert_idx + 6] = (GLfloat)(dst.x + dst.w);
		buf.vertices[texvert_idx + 7] = (GLfloat)(dst.y + dst.h);

		//update color buffer
		memcpy(buf.color + color_idx + 0, &color, 4);
		memcpy(buf.color + color_idx + 4, &color, 4);
		memcpy(buf.color + color_idx + 8, &color, 4);
		memcpy(buf.color + color_idx + 12, &color, 4);

		//update index buffer
		buf.index[index_idx + 0] = element_idx + 0;
		buf.index[index_idx + 1] = element_idx + 1;
		buf.index[index_idx + 2] = element_idx + 2;
		buf.index[index_idx + 3] = element_idx + 2;
		buf.index[index_idx + 4] = element_idx + 3;
		buf.index[index_idx + 5] = element_idx + 1;
#pragma warning( pop )
		//LOG_TRACE("Pushed quad buf.i=%d", buf.i);
	}

	void Renderer::Flush()
	{
		//LOG_TRACE("Atempted flush with buf.i=%d", buf.i);

		if (buf.i == 0)
		{
			return;
		}

		glViewport(0, 0, _width, _height);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.f, _width, _height, 0.f, -1.f, 1.f);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glTexCoordPointer(2, GL_FLOAT, 0, buf.texture);
		glVertexPointer(2, GL_FLOAT, 0, buf.vertices);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, buf.color);
		glDrawElements(GL_TRIANGLES, buf.i * 6, GL_UNSIGNED_INT, buf.index);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		buf.i = 0;
	}

	void Renderer::Clear(mu_Color color)
	{
		Flush();
		glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
