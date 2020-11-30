#pragma once

#include "Core.h"
#include "microui.h"
#include "atlas/Atlas.inl"

#define RENDER_BUFFER_SIZE 16384

namespace Q2Compilers {

	class Renderer
	{
	public:
		Renderer(GLFWwindow* window);
		~Renderer();
		void ProcessCommands(mu_Context *context, mu_Color clearColor);
	private:
		void CmdText(const char *text, mu_Vec2 pos, mu_Color color);
		void CmdRect(mu_Rect rect, mu_Color color);
		void CmdIcon(int id, mu_Rect rect, mu_Color color);
		void CmdClip(mu_Rect rect);
		void PushQuad(mu_Rect dst, mu_Rect src, mu_Color color);
		void Flush();
		void Clear(mu_Color color);
		int _width;
		int _height;
		GLFWwindow* _window;
		GLuint _atlasTex;

		struct RenderBuffer {
			GLfloat texture[RENDER_BUFFER_SIZE * 8];
			GLfloat vertices[RENDER_BUFFER_SIZE * 8];
			GLubyte color[RENDER_BUFFER_SIZE * 16];
			GLuint index[RENDER_BUFFER_SIZE * 8];
			int i = 0;
		} buf;
	};
}
