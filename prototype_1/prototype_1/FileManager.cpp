#include "Application.h"

void FileManager::ReadTextFromFile(const char * path, std::string *& buffer)
{
	std::string readText;
	hFile.open(path);

	if (hFile)
	{
		while (std::getline(hFile, line))
		{
			readText.append(line);
		}
	}
	
	else MessageBox(NULL, TEXT("Couldn't open file!"), TEXT("Error!"), NULL);

	hFile.close();
	buffer = &readText;
}

void FileManager::ReadTextFromFileW(const char * path, wchar_t *& buffer)
{
	std::string readText;
	wchar_t * readBuffer;
	hFile.open(path);

	if (hFile)
	{
		while (std::getline(hFile, line))
		{
			readText.append(line);
			readText.append("\r\n");
		}
	}

	else MessageBox(NULL, TEXT("Couldn't open file!"), TEXT("Error!"), NULL);

	hFile.close();
	size_t size = readText.size() + 1;
	readBuffer = new wchar_t[size];

	size_t outSize;
	mbstowcs_s(&outSize, readBuffer, size, readText.c_str(), size - 1);
	buffer = readBuffer;
}