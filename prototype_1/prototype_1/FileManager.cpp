#include "Application.h"

std::wstring FileManager::ReadTextFromFileW(const TCHAR * path)
{
	std::wstring readText;
	HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD error = GetLastError();
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	LPVOID viewAddress = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	char * pData;

	pData = (char*) viewAddress;
	if (pData)
	{
		std::string data = pData;
		readText.assign(data.begin(), data.end());
		UnmapViewOfFile(viewAddress);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		viewAddress = NULL;
		hFileMapping = NULL;
		hFile = NULL;
	}
	else
	{
		MessageBox(NULL, _T("File view returned NULL."), _T("Error"), NULL);
	}

	return readText;
}