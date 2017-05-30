#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <cstdlib>
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

struct TextData
{
	int textX, textY;
};

struct WindowSizeData
{
	int xPos, yPos;
	int width, height;
};

class IPrototypeForm
{
public:
	virtual void SetFormText(TCHAR * text) = 0;
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
	void SetFormText(wchar_t * text);
	wchar_t * GetFormText();
	void SetFormSize(WindowSizeData wsd);
};

class MainForm
{
private:
	HWND hWnd;
	RECT rc;
	TextData td;
	WindowSizeData wsd;
	TCHAR * currentText;

public:
	MainForm();
	void SetTextPosition(WPARAM wParam);
	void SetTextPosition(LPARAM lParam);
	TextData GetTextData();
	void SetCurrentText(TCHAR *);
	TCHAR* GetCurrentText();
	void DrawCurrentText();
	void SetHwnd(HWND hWnd);
	void SetWindowSizeData(WindowSizeData wsd);
	bool HandleKeyboardInput(WPARAM wParam);
	WindowSizeData GetWindowSizeData();
};

class FileManager
{
private:
	std::ifstream hFile;
	std::string line;
public:
	void ReadTextFromFile(const char * path, std::string *& buffer);
	void ReadTextFromFileW(const char * path, wchar_t *& buffer);
};