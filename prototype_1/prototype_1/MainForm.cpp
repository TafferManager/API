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
	case VK_DOWN:
		if (this->td.textY + 10 >= 0 && this->td.textY + 10 <= rc.bottom - 20)
		{
			InvalidateRect(hWnd, NULL, 1);
			SetCurrentText(_T("Going down... Nooo!"));
			this->td.textY += 5;
			UpdateWindow(hWnd);
		}
		break;
	case VK_RIGHT:
		if (this->td.textX + 10 >= 0 && this->td.textX + 5 <= rc.right - 20)
		{
			InvalidateRect(hWnd, NULL, 1);
			SetCurrentText(_T("Going right... Weee!"));	
			this->td.textX += 5;
			UpdateWindow(hWnd);
		}
		break;
	case VK_LEFT:
		if (this->td.textX - 10 >= 0)
		{
			InvalidateRect(hWnd, NULL, 1);
			SetCurrentText(_T("Going left... Whoops!"));
			this->td.textX -= 5;
			UpdateWindow(hWnd);
		}
		break;
	case VK_UP:
		if (this->td.textY - 10 >= 0)
		{
			InvalidateRect(hWnd, NULL, 1);
			SetCurrentText(_T("Going up... Yahoo!"));
			this->td.textY -= 5;
			UpdateWindow(hWnd);
		}
		break;
	case VK_RETURN:
		InvalidateRect(hWnd, NULL, 1);
		SetCurrentText(_T("Enter key pressed!"));
		UpdateWindow(hWnd);
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
	this->td.textX = GET_X_LPARAM(lParam);
	this->td.textY = GET_Y_LPARAM(lParam);

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