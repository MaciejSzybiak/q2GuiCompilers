#pragma once
#include <string>
namespace qvisn
{
	extern "C" {
		struct QvisData
		{
			int threads = 0;
			int fastvis = false;
			int verbose = false;
			int nosort = false;
			int cullerror = false;

			std::string mapPath;
		};

		int exec_qvis(QvisData data);
	}
}
