#include "Application.h"

#define ID_FILE_EXIT   9001
#define ID_STUFF_GO    9002
#define ID_CHANGE_TEXT 9003
#define ID_CHANGE_BACK 9004

WindowSizeData wsd;
IPrototypeForm * textForm;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void LoadComboBoxItems(HWND hWndComboBox);
WindowSizeData SetWindowSize(float width, float height);

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

	//mainForm = new MainForm();
	wsd = SetWindowSize(1280.f, 720.f);
	//mainForm->SetWindowSizeData(wsd);

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, wsd.xPos, wsd.yPos,
		 wsd.width, wsd.height, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		DWORD lastError = GetLastError();
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
	}

    textForm = new MainTextForm(hWnd, wsd);
	FileManager * fm = new FileManager;
	wchar_t * text;
	fm->ReadTextFromFileW("test.txt", text);
	textForm->SetFormText(text);

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
		/*HMENU hMenu, hSubMenu;

		hMenu = CreateMenu();

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_CHANGE_TEXT, _T("T&ext"));
		AppendMenu(hSubMenu, MF_STRING, ID_CHANGE_BACK, _T("Text &Back"));
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, _T("E&xit"));
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, _T("&File"));

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING | MF_GRAYED, ID_STUFF_GO, _T("&Go"));
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, _T("&Stuff"));

		SetMenu(hWnd, hMenu);*/
		break;
	case WM_SIZE:
		RECT rc;
		WindowSizeData wsd;
		GetClientRect(hWnd, &rc);
		wsd.width = rc.right;
		wsd.height = rc.bottom;
		textForm->SetFormSize(wsd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	//switch (message)
	//{
	///*case WM_CREATE:


	//	textForm = new MainTextForm(hWnd, wsd);
	//	break;
	//case WM_PAINT:
	//	hdc = BeginPaint(hWnd, &ps);
	//	EndPaint(hWnd, &ps);
	//	break;*/
	////case WM_MOUSEMOVE:
	////	mainForm->SetTextPosition(lParam);
	////	break;
	////case WM_COMMAND:
	////	switch (LOWORD(wParam))
	////	{
	////	case ID_FILE_EXIT:
	////		PostQuitMessage(0);
	////		break;
	////	case ID_CHANGE_TEXT:
	////		InvalidateRect(hWnd, NULL, 1);
	////		//mainForm->SetCurrentText(changedText);
	////		UpdateWindow(hWnd);
	////		return DefWindowProc(hWnd, message, wParam, lParam);
	////		break;
	////	case ID_CHANGE_BACK:
	////		InvalidateRect(hWnd, NULL, 1);
	////		//mainForm->SetCurrentText(greeting);
	////		UpdateWindow(hWnd);
	////		return DefWindowProc(hWnd, message, wParam, lParam);
	////		break;
	////	default:
	////		break;
	////	}
	////	break;
	//case WM_DESTROY:
	//	PostQuitMessage(0);
	//	break;
	//default:
	//	return DefWindowProc(hWnd, message, lParam, wParam);
	//}
	//return 0;
}

void LoadComboBoxItems(HWND hWndComboBox)
{
	TCHAR Planets[9][10] =
	{
		TEXT("Mercury"), TEXT("Venus"), TEXT("Terra"), TEXT("Mars"),
		TEXT("Jupiter"), TEXT("Saturn"), TEXT("Uranus"), TEXT("Neptune"),
		TEXT("Pluto??")
	};

	TCHAR A[16];
	int k = 0;

	memset(&A, 0, sizeof(A));
	for (k = 0; k <= 8; k += 1)
	{
		wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Planets[k]);

		// Add string to combobox.
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	// Send the CB_SETCURSEL message to display an initial item
	//  in the selection field
	SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
}

WindowSizeData SetWindowSize(float width, float height)
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