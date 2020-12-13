#pragma once

namespace Q2Compilers
{
	struct ArgOptions
	{
		bool instant = false;
		char mapName[128] = { 0 };
		char profile[128] = { 0 };
	};

	class ArgParser
	{
	public:
		static ArgOptions ParseArgs(int argc, char** argv);
	private:
		static int index;
	};
}
