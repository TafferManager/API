#include "Application.h"

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
