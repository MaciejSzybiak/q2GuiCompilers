#pragma once
#include <string>

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

	int exec_qbsp(QbspData data);
}
