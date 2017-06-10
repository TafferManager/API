#include "Application.h"

MainTextForm::MainTextForm(HWND parentHwnd, WindowSizeData wsd)
{
	this->currentText = NULL;
	this->parentHwnd = parentHwnd;

	LoadLibrary(TEXT("Msftedit.dll"));

	this->formHwnd = CreateWindowEx(0, TEXT("Edit"), TEXT(""),
		ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP,
		0, 20, wsd.width - 100, wsd.height - 100, this->parentHwnd, NULL, NULL, NULL);
}

void MainTextForm::SetFormText(LPCWSTR lpstrText)
{
	SendMessage(formHwnd, EM_SETLIMITTEXT, wcslen(lpstrText) + 100, 0);
	//SetWindowText(formHwnd, L"T");
	//SetWindowTextW(formHwnd, lpstrText);
}

void MainTextForm::SetFormText(std::string strData)
{
	SendMessage(formHwnd, EM_SETLIMITTEXT, strData.length() + 100, 0);
	SetWindowTextA(formHwnd, strData.c_str());
}

void MainTextForm::SetFormSize(WindowSizeData wsd)
{
	SetWindowPos(this->formHwnd, NULL, 0, 20, wsd.width - 100, wsd.height - 100, NULL);
}
