#include "Core.h"
#include "Config.h"


namespace Q2Compilers
{

	void to_json(json& j, const ConfigData& p)
	{
		j = json{
			{ "profile_last", p.profile_last},
		};
	}

	void from_json(const json& j, ConfigData& p)
	{
		j.at("profile_last").get_to(p.profile_last);
	}

	Config::Config()
	{
		if (!ReadConfig())
		{
			WriteConfig();
		}
	}

	void Config::SetLastProfile(std::string& name)
	{
		data.profile_last = name;
		WriteConfig();
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
