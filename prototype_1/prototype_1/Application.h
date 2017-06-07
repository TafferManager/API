#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <cstdlib>
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
	virtual void SetFormText(std::wstring wText) = 0;
	virtual TCHAR * GetFormText() = 0;
	virtual void SetFormSize(WindowSizeData wsd) = 0;
};

class MainTextForm : public IPrototypeForm
{
private:
	HWND parentHwnd;
	HWND formHwnd;
	wchar_t * currentText;
public:
	MainTextForm(HWND parentHwnd, WindowSizeData wsd);
	void SetFormText(std::wstring wText);
	wchar_t * GetFormText();
	void SetFormSize(WindowSizeData wsd);
};

class FileManager
{
private:
	std::wifstream hFile;
	std::wstring line;
public:
	LPWSTR ReadTextFromFileW(const TCHAR * path);
};