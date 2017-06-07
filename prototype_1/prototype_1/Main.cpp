#include "Application.h"

#define ID_FILE_EXIT   9001
#define ID_STUFF_GO    9002
#define ID_OPEN        9003

WindowSizeData wsd;
IPrototypeForm * textForm;
FileManager * fm;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
WindowSizeData SetWSDStruct(float width, float height);
void ShowOpenFileDialog(HWND p_hWnd);

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

	wsd = SetWSDStruct(1280.f, 720.f);

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, wsd.xPos, wsd.yPos,
		 wsd.width, wsd.height, NULL, NULL, hInstance, NULL);
	
	if (!hWnd)
	{
		DWORD lastError = GetLastError();
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
	}

    textForm = new MainTextForm(hWnd, wsd);
	fm = new FileManager;

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
	TCHAR greeting[] = _T("Original text.");
	TCHAR changedText[] = _T("Changed text.");

	switch (message)
	{
	case WM_CREATE:
		HMENU hMenu, hSubMenu;

		hMenu = CreateMenu();

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_OPEN, _T("O&pen"));
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, _T("E&xit"));
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, _T("&File"));

		SetMenu(hWnd, hMenu);
		break;
	case WM_SIZE:
		RECT rc;
		WindowSizeData wsd;
		GetClientRect(hWnd, &rc);
		wsd.width = rc.right;
		wsd.height = rc.bottom;
		if (textForm)
		{
			textForm->SetFormSize(wsd);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_OPEN:
			ShowOpenFileDialog(hWnd);
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

WindowSizeData SetWSDStruct(float width, float height)
{
	WindowSizeData wsd;

	int dpiX = 0;
	int dpiY = 0;
	HDC hdc = GetDC(NULL);
	if (hdc)
	{
		dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}
	wsd.width = static_cast<UINT>(ceil(width * dpiX / 96.f));
	wsd.height = static_cast<UINT>(ceil(height * dpiY / 96.f));
	wsd.xPos = 0;
	wsd.yPos = 0;

	return wsd;
}

void ShowOpenFileDialog(HWND p_hWnd)
{
	OPENFILENAME ofn;
	wchar_t szFile[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = p_hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//Display the Open dialog box.

	if (GetOpenFileName(&ofn) == TRUE)
	{
		LPWSTR wText = fm->ReadTextFromFileW(ofn.lpstrFile);
		textForm->SetFormText(wText);
		free(wText);
	}
}