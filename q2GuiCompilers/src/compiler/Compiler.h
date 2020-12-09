#pragma once
#include <thread>

namespace Q2Compilers
{
	class Compiler
	{
	public:
		bool Compile(CData *data, const std::string& mapPath);
		bool Update();
		CData& GetCompileData()
		{
			return compileData;
		}
		std::string& GetPath()
		{
			return path;
		}
		std::string& GetBspPath()
		{
			return pathBsp;
		}
		std::string& GetGamedir()
		{
			return gamedir;
		}
		std::string& GetModdir()
		{
			return moddir;
		}
	private:
		CData compileData;
		std::string path;
		std::string pathBsp;
		std::string gamedir;
		bool useModdir = false;
		std::string moddir;
		std::string executable;
		std::string copyTarget;
		std::queue<std::function<void()>> tasks;
		std::thread *currentTask;
		std::string execCommandLine;
		bool allowNext = false;
		bool failed = false;

		void BuildCompilationQueue();
		void SetPaths();
		bool ValidatePaths();

		bool FileExists(std::string& filename);
		bool DirectoryExists(std::string& directory);
	};
}

