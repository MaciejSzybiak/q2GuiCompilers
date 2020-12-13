#include "Core.h"
#include "data/CompilerData.h"
#include <algorithm>
#include "Compiler.h"
#include "blargh.h"
#include "qbspi.h"
#include "qvisi.h"

typedef int (*QbspFunc)(QbspData);
typedef int (*QvisFunc)(QvisData);
typedef int (*BlarghFunc)(BlarghData);

namespace Q2Compilers
{
	static QbspData GetQbspData(Compiler& c)
	{
		QbspData d;

		CData& compileData = c.GetCompileData();

		d.glView = compileData.qbsp_glView;
		d.leaktest = compileData.qbsp_leaktest;
		d.verbose = compileData.qbsp_verbose;
		d.nodetail = compileData.qbsp_nodetail;
		d.nowater = compileData.qbsp_nowater;
		d.noweld = compileData.qbsp_noweld;

		d.mapPath = c.GetPath();
		d.gamedir = c.GetGamedir();
		d.moddir = c.GetModdir();

		return d;
	}

	static QvisData GetQvisData(Compiler& c)
	{
		QvisData d;

		CData& compileData = c.GetCompileData();

		d.cullerror = compileData.qvis_cullerror;
		d.fastvis = compileData.qvis_fastvis;
		d.nosort = compileData.qvis_nosort;
		d.threads = compileData.qvis_threads;
		d.verbose = compileData.qvis_qvis_verbose;

		d.mapPath = c.GetPath();

		return d;
	}

	static BlarghData GetBlarghData(Compiler& c)
	{
		BlarghData d;

		CData& compileData = c.GetCompileData();

		d.dump = compileData.argh_dump;
		d.bounce = compileData.argh_bounce;
		d.verbose = compileData.argh_verbose;
		d.extra = compileData.argh_extra;
		d.update = compileData.argh_update;
		d.threads = compileData.argh_threads;
		d.chop = compileData.argh_chop;
		d.scale = compileData.argh_scale;
		d.direct = compileData.argh_direct;
		d.entity = compileData.argh_entity;
		d.texscale = compileData.argh_texscale;
		d.glView = compileData.argh_glView;
		d.nopvs = compileData.argh_nopvs;
		d.nocolor = compileData.argh_nocolor;
		d.saturation = compileData.argh_saturation;
		d.ambient = compileData.argh_ambient;
		d.minlight = compileData.argh_minlight;
		d.maxlight = compileData.argh_maxlight;
		d.gamma = compileData.argh_gamma;
		d.stylemin = compileData.argh_stylemin;
		d.radmin = compileData.argh_radmin;
		d.nobmodlight = compileData.argh_nobmodlight;
		d.nobouncefix = compileData.argh_nobouncefix;
		d.nobrightsurf = compileData.argh_nobrightsurf;
		d.nostopbleed = compileData.argh_nostopbleed;
		d.nocurve = compileData.argh_nocurve;
		d.nosplotchfix = compileData.argh_nosplotchfix;
		d.noradorigin = compileData.argh_noradorigin;
		d.lightwarp = compileData.argh_lightwarp;

		d.bspName = c.GetBspPath();
		d.gamedir = c.GetGamedir();
		d.moddir = c.GetModdir();

		return d;
	}

	bool Compiler::FileExists(const char* filename)
	{
		std::ifstream file(filename);
		return file.good();
	}

	bool Compiler::DirectoryExists(const char* directory)
	{
		return std::filesystem::exists(directory);
	}

	bool Compiler::Compile(CData* data, const char *mapPath)
	{
		compileData = *data;
		if (!compileData.enable_copy)
		{
			compileData.enable_exec = false;
		}

		SetPaths(mapPath);

		if (!ValidatePaths())
		{
			LOG_ERROR("Compilation failed.");
			return false;
		}

		BuildCompilationQueue();

		allowNext = true;
		failed = false;
		currentTask = nullptr;

		return true;
	}

	bool Compiler::Update()
	{
		if (failed)
		{
			if (currentTask)
			{
				currentTask->join();
			}
			return false;
		}

		if (allowNext)
		{
			allowNext = false;
			if (currentTask)
			{
				currentTask->join();
				delete currentTask;
			}

			if (tasks.empty())
			{
				//all done
				return false;
			}

			//next task
			currentTask = new std::thread(tasks.front());
			tasks.pop();
		}

		return true;
	}

	void Compiler::BuildCompilationQueue()
	{
		while (!tasks.empty())
		{
			tasks.pop();
		}

		//qbsp stage is mandatory
		QbspData qbspData = GetQbspData(*this);
		tasks.push([qbspData, this]()
		{
			try
			{
				LOG_INFO("TASK: qbsp");
				HINSTANCE handle = LoadLibraryA("compilers/qbsp.dll");
				if (handle == nullptr)
				{
					throw std::exception("Failed to load qbsp.dll!");
				}
				QbspFunc func = (QbspFunc)GetProcAddress(handle, "exec_qbsp");
				if (func == nullptr)
				{
					throw std::exception("Function \"exec_qbsp\" not found!");
				}
				func(qbspData);
				FreeLibrary(handle);
				allowNext = true;
			}
			catch (const std::exception& e)
			{
				LOG_ERROR("%s", e.what());
				failed = true;
			}
		});
		
		if (compileData.enable_qvis)
		{
			QvisData qvisData = GetQvisData(*this);
			tasks.push([qvisData, this]()
			{
				try
				{
					LOG_INFO("TASK: qvis");
					HINSTANCE handle = LoadLibraryA("compilers/qvis.dll");
					if (handle == nullptr)
					{
						throw std::exception("Failed to load qvis.dll!");
					}
					QvisFunc func = (QvisFunc)GetProcAddress(handle, "exec_qvis");
					if (func == nullptr)
					{
						throw std::exception("Function \"exec_qvis\" not found!");
					}
					func(qvisData);
					FreeLibrary(handle);
					allowNext = true;
				}
				catch (const std::exception& e)
				{
					LOG_ERROR("%s", e.what());
					failed = true;
				}
			});
		}

		if (compileData.enable_qrad)
		{
			BlarghData blarghData = GetBlarghData(*this);
			tasks.push([blarghData, this]()
			{
				try
				{
					LOG_INFO("TASK: blarghrad");
					HINSTANCE handle = LoadLibraryA("compilers/blarghrad.dll");
					if (handle == nullptr)
					{
						throw std::exception("Failed to load blarghrad.dll!");
					}
					BlarghFunc func = (BlarghFunc)GetProcAddress(handle, "exec_blarghrad");
					if (func == nullptr)
					{
						throw std::exception("Function \"exec_blarghrad\" not found!");
					}
					func(blarghData);
					FreeLibrary(handle);
					allowNext = true;
				}
				catch (const std::exception& e)
				{
					LOG_ERROR("%s", e.what());
					failed = true;
				}
			});
		}

		if (compileData.enable_copy)
		{
			tasks.push([this]()
			{
				try
				{
					LOG_INFO("TASK: copy");
					std::filesystem::copy_file(pathBsp, copyTarget, std::filesystem::copy_options::overwrite_existing);
					allowNext = true;
					LOG_INFO("Copied %s to %s", pathBsp, copyTarget);
				}
				catch (const std::exception& e)
				{
					LOG_ERROR("%s", e.what());
					failed = true;
				}
			});
		}

		if (compileData.enable_exec)
		{
			tasks.push([this]()
			{
				LOG_INFO("Executing %s", executable);

				STARTUPINFOA sInfo;
				PROCESS_INFORMATION pInfo;

				ZeroMemory(&sInfo, sizeof(sInfo));
				sInfo.cb = sizeof(sInfo);
				ZeroMemory(&pInfo, sizeof(pInfo));

				char args[C_ARGS_LENGTH + C_PATH_LENGTH];
				strcpy(args, executable);
				strcat(args, " ");
				strcat(args, execCommandLine);

				BOOL result = CreateProcessA(executable, args, NULL, NULL, false, 0, NULL, NULL, &sInfo, &pInfo);

				if (!result)
				{
					failed = true;
					CloseHandle(pInfo.hProcess);
					CloseHandle(pInfo.hThread);
					return;
				}

				LOG_INFO("Process %lu started", pInfo.dwProcessId);

				WaitForSingleObject(pInfo.hProcess, INFINITE);

				CloseHandle(pInfo.hProcess);
				CloseHandle(pInfo.hThread);

				LOG_INFO("Process ended");

				allowNext = true;
			});
		}
	}

	void Compiler::SetPaths(const char *mapPath)
	{
		memset(path, 0, sizeof(path));
		memset(pathBsp, 0, sizeof(pathBsp));
		memset(gamedir, 0, sizeof(gamedir));
		memset(moddir, 0, sizeof(moddir));
		memset(executable, 0, sizeof(executable));
		memset(copyTarget, 0, sizeof(copyTarget));
		memset(execCommandLine, 0, sizeof(execCommandLine));

		//map path
		if (mapPath[1] != ':')
		{
			strcat_s(path, std::filesystem::current_path().string().c_str());
			strcat_s(path, "\\");
		}
		strcat_s(path, mapPath);

		if (!EndsWith(path, ".map"))
		{
			strcat_s(path, ".map");
		}

		Replace(path, '/', '\\');

		//map name
		int start = LastIndexOf(path, '\\') + 1;
		int end = LastIndexOf(path, '.');
		char mapname[32] = { 0 };
		strncpy_s(mapname, path + start, (size_t)end - start);

		//bsp path
		strncpy_s(pathBsp, path, strlen(path) - 4);
		strcat_s(pathBsp, ".bsp");

		//q2 folder
		char baseDir[C_PATH_LENGTH] = { 0 };
		strcpy_s(baseDir, compileData.q2_directory);
		Replace(baseDir, '/', '\\');
		if (!EndsWith(baseDir, "\\"))
		{
			strcat_s(baseDir, "\\");
		}

		//moddir
		char moddirname[32] = { 0 };
		if (!strcmp(compileData.q2_modname, "baseq2") || !strlen(compileData.q2_modname))
		{
			useModdir = false;
		}
		else
		{
			useModdir = true;
			if (EndsWith(compileData.q2_modname, "\\") || EndsWith(compileData.q2_modname, "/"))
			{
				compileData.q2_modname[strlen(compileData.q2_modname) - 1] = '\0';
			}
			if (compileData.q2_modname[1] != ':')
			{
				strcpy_s(moddir, baseDir);
			}

			strcat_s(moddir, compileData.q2_modname);
			Replace(moddir, '/', '\\');
			//BROKEN
			start = LastIndexOf(moddir, '\\') + 1;
			strcpy_s(moddirname, moddir + start);

			strcat_s(moddir, "\\");
		}

		//gamedir
		strcpy_s(gamedir, baseDir);
		strcat_s(gamedir, "baseq2\\");

		//executable
		if (compileData.enable_exec)
		{
			//executable = "";
			if (compileData.q2_executable[1] != ':')
			{
				strcpy_s(executable, baseDir);
			}
			strcat_s(executable, compileData.q2_executable);

			//args
			if (useModdir)
			{
				strcpy_s(execCommandLine, "+set game ");
				strcat_s(execCommandLine, moddirname);
				strcat_s(execCommandLine, " ");
			}
			strcat_s(execCommandLine, "+map ");
			strcat_s(execCommandLine, mapname);
			strcat_s(execCommandLine, " ");
			strcat_s(execCommandLine, compileData.q2_args);
		}
	
		//copy target
		if (useModdir)
		{
			strcpy_s(copyTarget, moddir);
		}
		else
		{
			strcpy_s(copyTarget, gamedir);
		}
		strcat_s(copyTarget, "maps\\");
		strcat_s(copyTarget, mapname);
		strcat_s(copyTarget, ".bsp");
	}

	bool Compiler::ValidatePaths()
	{
		if (!FileExists(path))
		{
			LOG_ERROR("Map %s not found!", path);
			return false;
		}

		if (compileData.enable_copy)
		{
			if (!DirectoryExists(gamedir))
			{
				LOG_WARNING("Game directory not found!");
				compileData.enable_copy = false;
				compileData.enable_exec = false;
			}
			if (useModdir && !DirectoryExists(moddir))
			{
				LOG_WARNING("Mod directory not found!");
				compileData.enable_copy = false;
				compileData.enable_exec = false;
			}
		}

		if (compileData.enable_exec)
		{
			if (!FileExists(executable))
			{
				LOG_WARNING("Q2 executable not found!");
				compileData.enable_exec = false;
			}
		}
		return true;
	}
}
