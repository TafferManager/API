#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <tchar.h>

struct TextData
{
	int textX, textY;
};

class MainForm
{
private:
	HWND hWnd;
	RECT rc;
	TextData td;
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
};