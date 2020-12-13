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
