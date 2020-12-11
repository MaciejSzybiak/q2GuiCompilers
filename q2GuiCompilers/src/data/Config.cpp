#include "Core.h"
#include "Config.h"


namespace Q2Compilers
{

	void to_json(json& j, const ConfigData& p)
	{
		j = json{
			{ "profile_last", p.profile_last},
			{ "map_path", p.map_path},
		};
	}

	void from_json(const json& j, ConfigData& p)
	{
		j.at("profile_last").get_to(p.profile_last);
		j.at("map_path").get_to(p.map_path);
	}

	Config::Config()
	{
		if (!ReadConfig())
		{
			WriteConfig();
		}
	}

	bool Config::ReadConfig()
	{
		std::ifstream file("config.json");

		if (file.good())
		{
			file >> j;
			Deserialize();
			return true;
		}
		return false;
	}

	void Config::WriteConfig()
	{
		std::ofstream file("config.json", std::ios_base::out | std::ios_base::trunc);

		if (file.good())
		{
			Serialize();
			file << std::setw(4) << j << std::endl;
			file.close();

			LOG_TRACE("Saved configuration file.");
		}
	}

	void Config::Serialize()
	{
		j = data;
	}

	void Config::Deserialize()
	{
		try
		{
			data = j;
		}
		catch (nlohmann::detail::exception e)
		{
			LOG_WARNING("Config deserialization: %s", e.what());
		}
	}
}
