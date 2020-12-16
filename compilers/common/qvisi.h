#pragma once
#include <string>

#ifdef QVIS_DLL
#define QVISAPI extern "C" __declspec(dllexport)
#endif

struct QvisData
{
	int threads = 0;
	int fastvis = false;
	int verbose = false;
	int nosort = false;
	int cullerror = false;

	char mapPath[260] = { 0 };
};
