#include "Application.h"

void MainForm::DrawCurrentText()
{
	HDC hdcBuffer = GetDC(hWnd);
	BOOL result = TextOut(hdcBuffer, this->td.textX, this->td.textY, currentText, _tcslen(currentText));
	ReleaseDC(hWnd, hdcBuffer);
}

MainForm::MainForm()
{
	this->currentText = _T("");
	this->td.textX = 0;
	this->td.textY = 0;
}

void MainForm::SetTextPosition(WPARAM wParam)
{
	GetClientRect(hWnd, &rc);
	switch (LOWORD(wParam))
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
}

void MainForm::SetTextPosition(LPARAM lParam)
{
	std::stringstream ss;
	ss << "Mouse x is " << GET_X_LPARAM(lParam) << " and y is " << GET_Y_LPARAM(lParam);
	wchar_t * wcstring = new wchar_t[ss.str().size() + 1];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, ss.str().size() + 1, ss.str().c_str(), _TRUNCATE);

	InvalidateRect(hWnd, NULL, 1);
	SetCurrentText(wcstring);
	UpdateWindow(hWnd);
}

void MainForm::SetCurrentText(TCHAR * textPtr)
{
	this->currentText = textPtr;
}

TCHAR* MainForm::GetCurrentText()
{
	return this->currentText;
}

void MainForm::SetHwnd(HWND hWnd)
{
	this->hWnd = hWnd;
}

void MainForm::SetWindowSizeData(WindowSizeData wsd)
{
	this->wsd = wsd;
}

bool MainForm::HandleKeyboardInput(WPARAM wParam)
{
	switch(LOWORD(wParam))
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

WindowSizeData MainForm::GetWindowSizeData()
{
	return this->wsd;
}

MainTextForm::MainTextForm(HWND parentHwnd, WindowSizeData wsd)
{
	this->currentText = NULL;
	this->parentHwnd = parentHwnd;

	LoadLibrary(TEXT("Msftedit.dll"));

	this->formHwnd = CreateWindowEx(0, MSFTEDIT_CLASS, TEXT(""),
		ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP,
		0, 20, wsd.width - 100, wsd.height - 100, this->parentHwnd, NULL, NULL, NULL);

}

void MainTextForm::SetFormText(std::wstring wText)
{
	SetWindowText(this->formHwnd, wText.c_str());
}

wchar_t * MainTextForm::GetFormText()
{
	return currentText;
}

void MainTextForm::SetFormSize(WindowSizeData wsd)
{
	SetWindowPos(this->formHwnd, NULL, 0, 20, wsd.width - 100, wsd.height - 100, NULL);
}
