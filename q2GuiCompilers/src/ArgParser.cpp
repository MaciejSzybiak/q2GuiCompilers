/*
	This file is a part of q2GuiCompilers.
	Copyright (C) 2020  Maciej Szybiak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
			arg = argv[index];
			
			if (!strcmp(arg, "-instant"))
			{
				o.instant = true;
				LOG_INFO("Instant compile = true");
			}
			else if (!strcmp(arg, "-profile"))
			{
				if (index >= argc)
				{
					LOG_ERROR("Failed to parse -profile option.");
					break;
				}
				strcpy(o.profile, argv[index]);
				LOG_INFO("Profile = %s", o.profile);
			}
			else
			{
				strcpy(o.mapName, argv[index]);
				LOG_INFO("Map = %s", o.mapName);
			}
			index++;
		}

		return o;
	}
}
