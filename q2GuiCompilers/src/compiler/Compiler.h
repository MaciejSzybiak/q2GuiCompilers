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
#include <thread>

namespace Q2Compilers
{
	class Compiler
	{
	public:
		bool Compile(CData *data, const char* mapPath);
		bool Update();
		CData& GetCompileData()
		{
			return compileData;
		}
		const char* GetPath()
		{
			return path;
		}
		const char* GetBspPath()
		{
			return pathBsp;
		}
		const char* GetGamedir()
		{
			return gamedir;
		}
		const char* GetModdir()
		{
			return moddir;
		}
	private:
		CData compileData;
		bool useModdir = false;
		std::queue<std::function<void()>> tasks;
		std::thread *currentTask;
		bool allowNext = false;
		bool failed = false;

		char path[C_PATH_LENGTH] = { 0 };
		char pathBsp[C_PATH_LENGTH] = { 0 };
		char gamedir[C_PATH_LENGTH] = { 0 };
		char moddir[C_PATH_LENGTH] = { 0 };
		char executable[C_PATH_LENGTH] = { 0 };
		char copyTarget[C_PATH_LENGTH] = { 0 };
		char execCommandLine[C_ARGS_LENGTH * 2] = { 0 };

		void BuildCompilationQueue();
		void SetPaths(const char* mapPath);
		bool ValidatePaths();

		bool FileExists(const char *filename);
		bool DirectoryExists(const char* directory);
		bool EndsWith(const char* arr, const char* suffix)
		{
			size_t arrL = strlen(arr);
			size_t sufL = strlen(suffix);
			return (arrL >= sufL) && !strcmp(arr + arrL - sufL, suffix);
		}
		void Replace(char* arr, char from, char to)
		{
			size_t arrL = strlen(arr);
			for (size_t i = 0; i < arrL; i++)
			{
				if (arr[i] == from)
				{
					arr[i] = to;
				}
			}
		}
		int LastIndexOf(const char* arr, char c)
		{
			int ret = -1;
			int i = 0;
			while (arr[i] != '\0')
			{
				if (arr[i] == c)
				{
					ret = i;
				}
				i++;
			}
			return ret;
		}
		int FirstIndexOf(const char* arr, char c)
		{
			int i = 0;
			while (arr[i] != '\0')
			{
				if (arr[i] == c)
				{
					return i;
				}
			}
		}
	};
}

