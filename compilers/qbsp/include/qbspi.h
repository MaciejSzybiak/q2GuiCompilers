#pragma once
#include <string>

#ifdef QBSP_DLL
#define QBSPAPI extern "C" __declspec(dllexport)
#endif

extern "C" {
	struct QbspData
	{
		int glView = false;
		int verbose = false;
		int noweld = false;
		int nodetail = false;
		int nowater = false;
		int leaktest = false;

		std::string gamedir;
		std::string moddir;
		std::string mapPath;
	};
}
