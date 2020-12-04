#pragma once

#include "json.hpp"

using json = nlohmann::json;

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
		int   argh_chop = 0;
		int   argh_chopsky = 0;
		int   argh_chopwarp = 0;
		int   argh_choplight = 0;
		int   argh_chopcurve = 0;
		float argh_scale = 0;
		float argh_direct = 0;
		float argh_entity = 0;
		float argh_texscale = 0;
		int   argh_glView = false;
		int   argh_nopvs = false;
		int   argh_nocolor = false;
		float argh_saturation = 0;
		float argh_ambient = 0;
		float argh_minlight = 0;
		float argh_maxlight = 0;
		float argh_gamma = 0;
		float argh_stylemin = 0;
		float argh_radmin = 0;
		int   argh_nobmodlight = false;
		int   argh_nobouncefix = false;
		int   argh_nobrightsurf = false;
		int   argh_nostopbleed = false;
		int   argh_nocurve = false;
		int   argh_nosplotchfix = false;
		int   argh_noradorigin = false;

		std::string q2_directory = "";
		std::string q2_modname = "";
		std::string q2_executable = "";
		std::string q2_args = "";
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
		void Serialize();
		void Deserialize();
		json j;
		CData data;
	};
}
