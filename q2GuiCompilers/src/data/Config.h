#pragma once

namespace Q2Compilers
{

	struct ConfigData
	{
		char profile_last[C_PATH_LENGTH] = { 0 };
		char map_path[C_PATH_LENGTH] = { 0 };
	};

	class Config
	{
	public:
		Config();
		void WriteConfig();
		ConfigData data;
	private:
		bool ReadConfig();
	};
}
