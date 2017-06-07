#include "Application.h"

LPWSTR FileManager::ReadTextFromFileW(const TCHAR * path)
{
	LPWSTR wStr = 0;
	size_t wcharSize = 0;
	HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	LPVOID viewAddress = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	LPCCH data = (char*) viewAddress;
	if (data)
	{
		size_t dataSize = strlen(data);
		wcharSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, data, dataSize, NULL, 0);
		wStr = new WCHAR[wcharSize];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, data, dataSize, wStr, wcharSize);
		UnmapViewOfFile(viewAddress);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
	}
	else
	{
		MessageBox(NULL, _T("File view returned NULL."), _T("Error"), NULL);
	}

	return wStr;
}