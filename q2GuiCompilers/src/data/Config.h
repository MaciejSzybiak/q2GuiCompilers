#pragma once
#include "json.hpp"

using json = nlohmann::json;

namespace Q2Compilers
{

	struct ConfigData
	{
		std::string profile_last = "";
		std::string map_path = "";
	};

	class Config
	{
	public:
		Config();
		void WriteConfig();
		ConfigData* GetCurrentData()
		{
			return &data;
		}
	private:
		bool ReadConfig();
		void Serialize();
		void Deserialize();
		ConfigData data;
		json j;
	};
}
