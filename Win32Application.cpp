#include "Win32Application.h"

Win32Application::Win32Application(
	int width,
	int height,
	PAINT_EVENT_HANDLER paintHandler) 
	:
	m_hWnd(NULL),
	m_width(width),
	m_height(height),
	m_paintEventHadler(paintHandler)
{
	LoadStringW(GetModuleHandle(NULL), IDS_APP_TITLE, m_appName, MAX_LOADSTRING);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::Status st = Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

Win32Application::~Win32Application()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
}

int Win32Application::Run()
{
	CreateMainWindow();
	return RunMessageLoop();
}

void Win32Application::CreateMainWindow()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &Win32Application::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_appName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL_APP_ICON));

	RegisterClassEx(&wcex);


	m_hWnd = CreateWindow(m_appName, m_appName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, m_width, m_height, NULL, NULL, GetModuleHandle(NULL), NULL);

	SetProp(m_hWnd, PAINT_EVENTHANDLER_PROP_NAME, (HANDLE)(&m_paintEventHadler));

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);
}

int Win32Application::RunMessageLoop()
{
	HACCEL hAccelTable = LoadAccelerators(
		GetModuleHandle(NULL), 
		MAKEINTRESOURCE(IDC_WIN32APPLICATION));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT WINAPI Win32Application::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		PAINT_EVENT_HANDLER* handler =
			reinterpret_cast<PAINT_EVENT_HANDLER*>(GetProp(hWnd, PAINT_EVENTHANDLER_PROP_NAME));

		(*handler)(hdc);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
