#pragma once

#include "json.hpp"
using json = nlohmann::json;

namespace Q2Compilers {

	struct CData {
		//qbsp
		bool qbsp_glView = false;
		bool qbsp_verbose = false;
		bool qbsp_noweld = false;
		bool qbsp_nodetail = false;
		bool qbsp_nowater = false;
		bool qbsp_leaktest = false;

		//qvis
		int qvis_threads = 0;
		bool qvis_fastvis = false;
		bool qvis_qvis_verbose = false;
		bool qvis_nosort = false;
		bool qvis_cullerror = false;

		//argh
		bool argh_dump = false;
		int argh_bounce = 0;
		bool argh_verbose = false;
		bool argh_extra = false;
		bool argh_update = false;
		int argh_threads = 0;
		int argh_chop = 0;
		int argh_chopsky = 0;
		int argh_chopwarp = 0;
		int argh_choplight = 0;
		int argh_chopcurve = 0;
		float argh_scale = 0; //light brightness scale
		float argh_direct = 0; //surface light brightness scale
		float argh_entity = 0; //entity light brightness scale
		float argh_texscale = 0; //texture light brightness scale
		bool argh_glView = false;
		bool argh_nopvs = false;
		bool argh_nocolor = false; //grayscale lightmaps
		float argh_saturation = 0; //lightmap saturation
		float argh_ambient = 0; //ambient value
		float argh_minlight = 0;
		float argh_maxlight = 0;
		float argh_gamma = 0;
		float argh_stylemin = 0;
		float argh_radmin = 0;
		bool argh_nobmodlight = false;
		bool argh_nobouncefix = false;
		bool argh_nobrightsurf = false;
		bool argh_nostopbleed = false;
		bool argh_nocurve = false;
		bool argh_nosplotchfix = false;
		bool argh_noradorigin = false;

		std::string q2_directory = "";
		std::string q2_modname = "";
		std::string q2_executable = "";
		std::string q2_args = "";
	};

	class CompilerData {
	public:
		bool LoadFromFile(std::string filename);
		CData* GetCurrentData() {
			return &data;
		}
		void SaveFile(std::string filename);
	private:
		void Serialize();
		void Deserialize();
		json j;
		CData data;
	};
}
