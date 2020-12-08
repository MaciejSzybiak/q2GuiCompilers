#include "Core.h"
#include "data/CompilerData.h"
#include <algorithm>
#include "Compiler.h"
#include "blargh.h"
#include "qbspi.h"
#include "qvisi.h"

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

	static qvisn::QvisData GetQvisData(Compiler& c)
	{
		qvisn::QvisData d;

		CData& compileData = c.GetCompileData();

		d.cullerror = compileData.qvis_cullerror;
		d.fastvis = compileData.qvis_fastvis;
		d.nosort = compileData.qvis_nosort;
		d.threads = compileData.qvis_threads;
		d.verbose = compileData.qvis_qvis_verbose;

		d.mapPath = c.GetPath();

		return d;
	}

	static blarghrad::BlarghData GetBlarghData(Compiler& c)
	{
		blarghrad::BlarghData d;

		CData& compileData = c.GetCompileData();

		d.dump = compileData.argh_dump;
		d.bounce = compileData.argh_bounce;
		d.verbose = compileData.argh_verbose;
		d.extra = compileData.argh_extra;
		d.update = compileData.argh_update;
		d.threads = compileData.argh_threads;
		d.chop = compileData.argh_chop;
		d.chopsky = compileData.argh_chopsky;
		d.chopwarp = compileData.argh_chopwarp;
		d.choplight = compileData.argh_choplight;
		d.chopcurve = compileData.argh_chopcurve;
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
		path = mapPath;

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
		//qbsp stage is mandatory
		QbspData qbspData = GetQbspData(*this);
		tasks.push([qbspData, this]()
		{
			try
			{
				LOG_INFO("TASK: qbsp");
				exec_qbsp(qbspData);
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
			qvisn::QvisData qvisData = GetQvisData(*this);
			tasks.push([qvisData, this]()
			{
				try
				{
					LOG_INFO("TASK: qvis"); 
					exec_qvis(qvisData);
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
			blarghrad::BlarghData blarghData = GetBlarghData(*this);
			tasks.push([blarghData, this]()
			{
				try
				{
					LOG_INFO("TASK: blarghrad");
					exec_blarghrad(blarghData);
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
				LOG_INFO("Execute not implemented yet.");
			});
		}
	}

	void Compiler::SetPaths()
	{
		//map path
		std::replace(path.begin(), path.end(), '/', '\\');

		//bsp path
		pathBsp = path.substr(0, path.length() - 4) + ".bsp";

		//q2 folder
		std::string s = compileData.q2_directory.c_str();
		std::replace(s.begin(), s.end(), '/', '\\');
		if (!s.ends_with("\\"))
		{
			s += "\\";
		}

		//moddir
		if (compileData.q2_modname.c_str() == "baseq2" || compileData.q2_modname.empty())
		{
			useModdir = false;
		}
		else
		{
			useModdir = true;
			moddir = s;
			moddir = moddir.append(std::string(compileData.q2_modname.c_str()));
			moddir = moddir.append(std::string("\\"));
		}

		//gamedir
		gamedir = s;
		gamedir = gamedir.append(std::string("baseq2\\"));
		//executable
		executable = s;
		executable = executable.append(std::string(compileData.q2_executable.c_str()));
	
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
