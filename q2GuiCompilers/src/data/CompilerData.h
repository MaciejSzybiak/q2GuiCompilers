#pragma once

namespace Q2Compilers
{

	struct CData
	{
		//qbsp
		int   qbsp_glView = false;
		int   qbsp_verbose = false;
		int   qbsp_noweld = false;
		int   qbsp_nodetail = false;
		int   qbsp_nowater = false;
		int   qbsp_leaktest = false;

		//qvis
		int   qvis_threads = 0;
		int   qvis_fastvis = false;
		int   qvis_qvis_verbose = false;
		int   qvis_nosort = false;
		int   qvis_cullerror = false;

		//argh
		int   argh_dump = false;
		int   argh_bounce = 0;
		int   argh_verbose = false;
		int   argh_extra = false;
		int   argh_update = false;
		int   argh_threads = 0;
		int   argh_chop = 128;
		float argh_scale = 1;
		float argh_direct = 1;
		float argh_entity = 1;
		float argh_texscale = 1;
		int   argh_glView = false;
		int   argh_nopvs = false;
		int   argh_nocolor = false;
		float argh_saturation = 1;
		float argh_ambient = 0;
		float argh_minlight = 0;
		float argh_maxlight = 0;
		float argh_gamma = 1;
		float argh_stylemin = 0;
		float argh_radmin = 0;
		int   argh_nobmodlight = false;
		int   argh_nobouncefix = false;
		int   argh_nobrightsurf = false;
		int   argh_nostopbleed = false;
		int   argh_nocurve = false;
		int   argh_nosplotchfix = false;
		int   argh_noradorigin = false;
		int   argh_lightwarp = false;

		int   enable_qvis = true;
		int   enable_qrad = true;
		int   enable_copy = true;
		int   enable_exec = true;

		char q2_directory[C_PATH_LENGTH];
		char q2_modname[C_PATH_LENGTH];
		char q2_executable[C_PATH_LENGTH];
		char q2_args[C_ARGS_LENGTH];
	};

	class CompilerData
	{
	public:
		bool LoadFromFile(const std::string& filename);
		CData* GetCurrentData()
		{
			return &data;
		}
		void SaveFile(const std::string& filename);
	private:
		CData data;
	};
}
