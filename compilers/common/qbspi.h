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

		char gamedir[260] = { 0 };
		char  moddir[260] = { 0 };
		char  mapPath[260] = { 0 };
	};
}
