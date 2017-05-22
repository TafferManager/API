#include "Application.h"

#define ID_FILE_EXIT   9001
#define ID_STUFF_GO    9002
#define ID_CHANGE_TEXT 9003
#define ID_CHANGE_BACK 9004

MainForm * mainForm;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static TCHAR szWindowClass[] = _T("win32app");
	static TCHAR szTitle[] = _T("Win32 Application");

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}

	mainForm = new MainForm();

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		DWORD lastError = GetLastError();
		MessageBox(NULL, LPCWSTR("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Original text.");
	TCHAR changedText[] = _T("Changed text.");

	switch (message)
	{
	case WM_CREATE:
		HMENU hMenu, hSubMenu;

		mainForm->SetHwnd(hWnd);

		hMenu = CreateMenu();

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_CHANGE_TEXT, _T("T&ext"));
		AppendMenu(hSubMenu, MF_STRING, ID_CHANGE_BACK, _T("Text &Back"));
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, _T("E&xit"));
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, _T("&File"));

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING | MF_GRAYED, ID_STUFF_GO, _T("&Go"));
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, _T("&Stuff"));

		SetMenu(hWnd, hMenu);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		mainForm->DrawCurrentText();
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		mainForm->SetTextPosition(wParam);
		break;
	case WM_MOUSEMOVE:
		mainForm->SetTextPosition(lParam);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case ID_CHANGE_TEXT:
			InvalidateRect(hWnd, NULL, 1);
			mainForm->SetCurrentText(changedText);
			UpdateWindow(hWnd);
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case ID_CHANGE_BACK:
			InvalidateRect(hWnd, NULL, 1);
			mainForm->SetCurrentText(greeting);
			UpdateWindow(hWnd);
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		default:
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}