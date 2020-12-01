#pragma once
#include "json.hpp"

using json = nlohmann::json;

namespace Q2Compilers
{

	struct ConfigData
	{
		std::string profile_last = "";
	};

	class Config
	{
	public:
		Config();
		void SetLastProfile(std::string name);
		ConfigData* GetCurrentData()
		{
			return &data;
		}
	private:
		bool ReadConfig();
		void WriteConfig();
		void Serialize();
		void Deserialize();
		ConfigData data;
		json j;
	};
}
