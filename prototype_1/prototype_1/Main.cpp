#include "Application.h"
#include "Tracer.h"

#define ID_FILE_EXIT   9001
#define ID_FILE_OPEN   9002

WindowSizeData wsd;
IPrototypeForm * p_textForm;
FileManager * p_fm;
ProtoTracer * p_protoTracer;
std::stringstream ss("");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
WindowSizeData SetWSDStruct(float width, float height);
void ShowOpenFileDialog(HWND p_hWnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static TCHAR szWindowClass[] = _T("win32app");
	static TCHAR szTitle[] = _T("Prototype TextEd 0.9.1");

	p_protoTracer = new ProtoTracer;

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
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
	}

	p_protoTracer->WriteLogEntry("Main window has been successfully created.");
    p_textForm = new MainTextForm(hWnd, hInstance, wsd);

	if (p_textForm)
		p_protoTracer->WriteLogEntry("Text form has been successfully created.");
	p_fm = new FileManager;

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
	switch (message)
	{
	case WM_CREATE:
		HMENU hMenu, hSubMenu;

		hMenu = CreateMenu();

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, _T("O&pen"));
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
		if (p_textForm)
		{
			p_textForm->SetFormSize(wsd);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
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
		p_protoTracer->WriteLogEntry("Exiting prototype_1...");
		p_protoTracer->~ProtoTracer();
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
		char buffer[300];
		char filename[260];
		size_t convertedChars;
		wcstombs_s(&convertedChars, filename, ofn.lpstrFile, 200);
		sprintf_s(buffer, 300, "File %s has been successfully opened.", filename);
		p_protoTracer->WriteLogEntry(buffer);
		//LPWSTR wText = p_fm->ReadTextFromFileW(ofn.lpstrFile);
		std::string strData = p_fm->ReadTextFromFile(ofn.lpstrFile);
		ZeroMemory(buffer, 300);
		p_textForm->SetFormText(strData);

		if (GetLastError() == (DWORD)0)
		{
			p_protoTracer->WriteLogEntry("Text data successfully loaded to text form, going to free text data from memory...");
		}

		else
		{
			ZeroMemory(buffer, 300);
			sprintf_s(buffer, 300, "Error %d occured when setting text to the text form.", (int)GetLastError());
			p_protoTracer->WriteLogEntry(buffer);
		}
	}
}