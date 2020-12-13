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
#include "Config.h"


namespace Q2Compilers
{
	Config::Config()
	{
		if (!ReadConfig())
		{
			WriteConfig();
		}
	}

	bool Config::ReadConfig()
	{
		wchar_t buf[C_PATH_LENGTH] = { 0 };
		wcscpy_s(buf, dataPath);
		CreateDirectoryW(buf, NULL);
		wcscat_s(buf, L"appdata");

		FILE* f = _wfopen(buf, L"r");

		if (f)
		{
			fread(&data, sizeof(data), 1, f);
			fclose(f);

			LOG_TRACE("Opened configuration file");

			return true;
		}

		return false;
	}

	void Config::WriteConfig()
	{
		wchar_t buf[C_PATH_LENGTH] = { 0 };
		wcscpy_s(buf, dataPath);
		CreateDirectoryW(buf, NULL);
		wcscat_s(buf, L"appdata");

		FILE* f = _wfopen(buf, L"w");

		if (f)
		{
			fwrite(&data, sizeof(data), 1, f);
			fclose(f);

			LOG_TRACE("Saved configuration file");
		}
	}
}
