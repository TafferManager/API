#include "Application.h"
#include "Tracer.h"

extern ProtoTracer * p_protoTracer;

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
		char buffer[200];
		size_t dataSize = strlen(data);
		sprintf_s(buffer, 200, "File has been mapped, %i bytes of data is ready for reading.", dataSize * sizeof(char));
		p_protoTracer->WriteLogEntry(buffer);
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

std::string FileManager::ReadTextFromFile(const TCHAR * path)
{
	LPCSTR lpcStr = 0;
	size_t wcharSize = 0;
	std::string strData;
	HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	LPVOID viewAddress = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	lpcStr = (char*)viewAddress;

	if (lpcStr)
	{
		char buffer[200];
		size_t dataSize = strlen(lpcStr);
		sprintf_s(buffer, 200, "File has been mapped, %i bytes of data is ready for reading.", dataSize * sizeof(char));
		p_protoTracer->WriteLogEntry(buffer);
		strData = lpcStr;
		strData += '\0';
		UnmapViewOfFile(viewAddress);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
	}

	return strData;
}