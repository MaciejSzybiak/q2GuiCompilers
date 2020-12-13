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

