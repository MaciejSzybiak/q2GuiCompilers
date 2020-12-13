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

	bool Compiler::FileExists(std::string& filename)
	{
		std::ifstream file(filename);
		return file.good();
	}

	bool Compiler::DirectoryExists(std::string& directory)
	{
		return std::filesystem::exists(directory);
	}

	bool Compiler::Compile(CData* data, const std::string& mapPath)
	{
		compileData = *data;
		if (!compileData.enable_copy)
		{
			compileData.enable_exec = false;
		}

		path = "";
		if (mapPath[1] != ':')
		{
			path = path.append(std::filesystem::current_path().string()).append("\\");
		}
		path = path.append(std::string(mapPath.c_str()));
		if (!path.ends_with(".map"))
		{
			path = path.append(".map");
		}

		if (!FileExists(path))
		{
			LOG_ERROR("Map %s not found!", path.c_str());
			return false;
		}

		SetPaths();

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
					LOG_INFO("Copied %s to %s", pathBsp.c_str(), copyTarget.c_str());
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
				LOG_INFO("Executing %s", executable.c_str());

				STARTUPINFOA sInfo;
				PROCESS_INFORMATION pInfo;

				ZeroMemory(&sInfo, sizeof(sInfo));
				sInfo.cb = sizeof(sInfo);
				ZeroMemory(&pInfo, sizeof(pInfo));

				char args[2048];
				strcpy(args, executable.c_str());
				strcat(args, " ");
				strcat(args, execCommandLine.c_str());

				BOOL result = CreateProcessA(executable.c_str(), args, NULL, NULL, false, 0, NULL, NULL, &sInfo, &pInfo);

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

	void Compiler::SetPaths()
	{
		//map path
		std::replace(path.begin(), path.end(), '/', '\\');

		//map name
		size_t maplast = path.find_last_of("\\") + 1;
		std::string mapname = path.substr(maplast, path.length() - 4 - maplast);

		//bsp path
		pathBsp = path.substr(0, path.length() - 4) + ".bsp";

		//q2 folder
		std::string s = std::string(compileData.q2_directory);
		std::replace(s.begin(), s.end(), '/', '\\');
		if (!s.ends_with("\\"))
		{
			s += "\\";
		}

		//moddir
		if (!strcmp(compileData.q2_modname, "baseq2") || !strlen(compileData.q2_modname))
		{
			useModdir = false;
		}
		else
		{
			useModdir = true;
			moddir = "";
			if (compileData.q2_modname[1] != ':')
			{
				moddir = moddir.append(s);
			}
			moddir = moddir.append(compileData.q2_modname);
			std::replace(moddir.begin(), moddir.end(), '/', '\\');
			if (!moddir.ends_with("\\"))
			{
				moddir = moddir.append("\\");
			}
		}

		//gamedir
		gamedir = s;
		gamedir = gamedir.append("baseq2\\");

		//executable
		if (compileData.enable_exec)
		{
			executable = "";
			if (compileData.q2_executable[1] != ':')
			{
				executable = executable.append(s);
			}
			executable = executable.append(compileData.q2_executable);

			//args
			execCommandLine = "";
			if (useModdir)
			{
				execCommandLine = execCommandLine.append("+set game ");
				execCommandLine = execCommandLine.append(compileData.q2_modname);
				execCommandLine = execCommandLine.append(" ");
			}
			execCommandLine = execCommandLine.append("+map ");
			execCommandLine = execCommandLine.append(mapname);
			execCommandLine = execCommandLine.append(" ");
			execCommandLine = execCommandLine.append(compileData.q2_args);
		}
	
		//copy target
		size_t start = path.find_last_of("\\") + 1;
		size_t length = path.find_last_of(".") - start;
		copyTarget = (useModdir ? moddir : gamedir);
		copyTarget = copyTarget.append(std::string("maps\\"));
		copyTarget = copyTarget.append(path.substr(start, length));
		copyTarget = copyTarget.append(std::string(".bsp"));
	}

	bool Compiler::ValidatePaths()
	{
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
