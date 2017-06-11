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
		sprintf_s(buffer, 200, "[INFO] File has been mapped, %i bytes of data is ready for reading.", dataSize * sizeof(char));
		p_protoTracer->WriteLogEntry(buffer);
		wcharSize = MultiByteToWideChar(CP_UTF8, 0, data, dataSize, NULL, 0);
		wStr = new WCHAR[wcharSize];
		MultiByteToWideChar(CP_UTF8, 0, data, dataSize, wStr, wcharSize);
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
		sprintf_s(buffer, 200, "[INFO] File has been mapped, %i bytes of data is ready for reading.", dataSize * sizeof(char));
		p_protoTracer->WriteLogEntry(buffer);
		strData = lpcStr;
		strData += '\0';
		UnmapViewOfFile(viewAddress);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
	}

	else
	{
		p_protoTracer->WriteLogEntry("[WARNING] Tried to open an empty file!");
	}
	return strData;
}

void FileManager::SaveTextToFile(const TCHAR * path, LPSTR & text)
{
	char buffer[500];
	char filename[260];
	size_t convertedChars = 0;
	DWORD dwBytesWritten = 0;

	HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD err = GetLastError();

	WriteFile(hFile, text, strlen(text) * sizeof(char), &dwBytesWritten, NULL);
	wcstombs_s(&convertedChars, filename, path, 260);
	sprintf_s(buffer, 500, "[INFO] %d bytes written to file: %s", (int)dwBytesWritten, filename);
	p_protoTracer->WriteLogEntry(buffer);
	CloseHandle(hFile);
}