#pragma once

namespace Q2Compilers
{
	struct ArgOptions
	{
		bool instant = false;
		std::string mapName;
		std::string profile;
	};

	class ArgParser
	{
	public:
		static ArgOptions ParseArgs(int argc, char** argv);
	private:
		static int index;
	};
}
