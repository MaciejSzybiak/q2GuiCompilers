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
#include "CompilerData.h"

namespace Q2Compilers
{
	bool CompilerData::LoadFromFile(const std::string& filename)
	{
		wchar_t buf[C_PATH_LENGTH] = { 0 };
		wcscpy_s(buf, dataPath);
		wcscat_s(buf, L"profiles\\");

		size_t size = strlen(filename.c_str()) + 1;
		wchar_t* wName = new wchar_t[size];
		mbstowcs(wName, filename.c_str(), size);
		wcscat_s(buf, wName);
		wcscat_s(buf, W_PROFILE_EXTENSION_STR);
		delete[] wName;

		FILE* f = _wfopen(buf, L"r");

		if (f)
		{
			fread(&data, sizeof(data), 1, f);
			fclose(f);

			LOG_TRACE("Loaded profile %s", filename.c_str());

			return true;
		}
		return false;
	}

	void CompilerData::SaveFile(const std::string& filename)
	{
		wchar_t buf[C_PATH_LENGTH] = { 0 };

		wcscpy_s(buf, dataPath);
		wcscat_s(buf, L"profiles\\");
		CreateDirectoryW(buf, NULL);

		size_t size = strlen(filename.c_str()) + 1;
		wchar_t* wName = new wchar_t[size];
		mbstowcs(wName, filename.c_str(), size);
		wcscat_s(buf, wName);
		wcscat_s(buf, W_PROFILE_EXTENSION_STR);
		delete[] wName;

		FILE* f = _wfopen(buf, L"w");

		if (f)
		{
			fwrite(&data, sizeof(data), 1, f);
			fclose(f);

			LOG_TRACE("Saved profile %s", filename.c_str());
		}
	}
}

