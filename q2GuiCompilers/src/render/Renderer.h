/*
	This file is a part of q2GuiCompilers.
	Copyright (C) 2020  Maciej Szybiak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "microui.h"

#define RENDER_BUFFER_SIZE 16384

namespace Q2Compilers
{
	class Renderer
	{
	public:
		Renderer(GLFWwindow* window);
		~Renderer();
		void ProcessCommands(mu_Context* context, mu_Color clearColor);
		static int GetTextWidth(const char* text, int len);
		void Finish(); //called right before swapping buffers
	private:
		void CmdText(const char* text, mu_Vec2 pos, mu_Color color);
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

		struct RenderBuffer
		{
			GLfloat texture[RENDER_BUFFER_SIZE * 8] = { 0 };
			GLfloat vertices[RENDER_BUFFER_SIZE * 8] = { 0 };
			GLubyte color[RENDER_BUFFER_SIZE * 16] = { 0 };
			GLuint index[RENDER_BUFFER_SIZE * 8] = { 0 };
			int i = 0;
		} buf;
	};
}
