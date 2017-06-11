#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <cstdlib>
#include <ctime>
#include <Richedit.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <sstream>
#include <string>
#include <tchar.h>
#include <vector>

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE) &__ImageBase)
#endif // !HINST_THISCOMPONENT

struct WindowSizeData
{
	int xPos, yPos;
	int width, height;
};

class IPrototypeForm
{
public:
	virtual void SetFormText(LPCWSTR lpstrText) = 0;
	virtual void SetFormText(std::string strData) = 0;
	virtual void SetFormSize(WindowSizeData wsd) = 0;
	virtual LPSTR GetFormText() = 0;
};

class MainTextForm : public IPrototypeForm
{
private:
	HWND parentHwnd;
	HWND formHwnd;
	wchar_t * currentText;
public:
	MainTextForm(HWND parentHwnd, HINSTANCE hInstance, WindowSizeData wsd);
	void SetFormText(LPCWSTR lpstrText);
	void SetFormText(std::string strData);
	void SetFormSize(WindowSizeData wsd);
	LPSTR GetFormText();
};

class FileManager
{
private:
	std::wifstream hFile;
	std::wstring line;
public:
	LPWSTR ReadTextFromFileW(const TCHAR * path);
	std::string ReadTextFromFile(const TCHAR * path);
	void SaveTextToFile(const TCHAR * path, LPSTR & text);
};