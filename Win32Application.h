#pragma once

#include "Resource.h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include <process.h> 
#include <iostream> 
#include <functional> 


#define MAX_LOADSTRING 100
#define PAINT_EVENTHANDLER_PROP_NAME L"OnPaint"

typedef std::function<void(HDC)> PAINT_EVENT_HANDLER;

class DefaultOnPaintEventHandler
{
public:
	void operator()(HDC hdc) {};
};

class Win32Application
{
public:
	Win32Application(
		const int width,
		const int height,
		PAINT_EVENT_HANDLER paintHandler = DefaultOnPaintEventHandler{});

	~Win32Application();

	int Run();

private:
	void CreateMainWindow();
	int RunMessageLoop();

	static LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;
	int m_width;
	int m_height;
	WCHAR m_appName[MAX_LOADSTRING];
	ULONG_PTR m_gdiplusToken;
protected:
	PAINT_EVENT_HANDLER m_paintEventHadler;
};


