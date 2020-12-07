#include "Core.h"
#include "ArgParser.h"

namespace Q2Compilers
{
	int ArgParser::index;

	ArgOptions ArgParser::ParseArgs(int argc, char** argv)
	{
		ArgOptions o;
		char* arg;

		if (argc < 2)
		{
			return o;
		}

		index = 1;

		LOG_INFO("Parsing arguments...");

		while (index < argc)
		{
			arg = argv[index++];
			
			if (!strcmp(arg, "-instant"))
			{
				o.instant = true;
				LOG_INFO("Instant copmpile = true");
			}
			else if (!strcmp(arg, "-profile"))
			{
				if (index >= argc)
				{
					LOG_ERROR("Failed to parse -profile option.");
					break;
				}
				o.profile = argv[index++];
				LOG_INFO("Profile = %s", o.profile.c_str());
			}
			else
			{
				o.mapName = arg;
				LOG_INFO("Map = %s", o.mapName.c_str());
			}
		}

		return o;
	}
}
