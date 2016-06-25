#ifndef TOOLBAG_WINDOW_WIN32_H
#define TOOLBAG_WINDOW_WIN32_H
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>

struct Window
{
	HWND hwnd = 0;
    HINSTANCE hInst = 0;
    HDC dc = 0;
	bool exists = false;
	bool quitOnDestroy = false;

	Window() = default;
	Window(const Window&) = delete;
	~Window();

	const bool Create(const char *windowTitle, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const bool initForGL = false, const bool autoShow = true);
	const bool NextMessage(MSG *msg, const bool thisWindowOnly = false);
	void Destroy();
	void Show();
	void Hide();

	static void UnusedMessage(const MSG *msg);
};
#endif

#ifdef TOOLBAG_WINDOW_WIN32_IMPL
#ifndef TOOLBAG_WINDOW_WIN32_IMPL_GUARD
#define TOOLBAG_WINDOW_WIN32_IMPL_GUARD
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

__forceinline HWND ParamsToHwnd(WPARAM wParam, LPARAM lParam)
{
#ifdef _M_X64
		return (HWND)(((uint64_t)wParam << 32) | (uint64_t)lParam);
#else
		return (HWND)(((uint32_t)wParam << 16) | (uint32_t)lParam);
#endif
}

__forceinline void HwndToParams(HWND hwnd, WPARAM *wParam, LPARAM *lParam)
{
#ifdef _M_X64
		*wParam = (UINT)(((uint64_t)hwnd & 0xFFFFFFFF00000000) >> 32);
		*lParam = (UINT)((uint64_t)hwnd & 0x00000000FFFFFFFF);
#else
		*wParam = (UINT)((uint32_t)hwnd & 0xFFFF0000) >> 16;
		*lParam = (UINT)((uint32_t)hwnd & 0x0000FFFF);
#endif
}

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			LPARAM l;
			WPARAM w;
			HwndToParams(hwnd, &w, &l);
			PostMessageA(NULL, WM_DESTROY, w, l);
			return 0;

		default:
			return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
}

const bool Window::Create(const char *windowTitle, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const bool initForGL, const bool autoShow)
{
	HINSTANCE hinst = HINST_THISCOMPONENT;
	WNDCLASSA cl = {};
    if (initForGL)
        cl.style = CS_OWNDC;
	cl.lpfnWndProc = WndProc;
	cl.hInstance = hinst;
	cl.lpszClassName = "window";
	ATOM c = RegisterClassA(&cl);
	if (c == 0)
		return false;

    uint32_t wx = (x == 0 ? CW_USEDEFAULT : x);
    uint32_t wy = (y == 0 ? CW_USEDEFAULT : y);
	hwnd = CreateWindowA("window", windowTitle, WS_OVERLAPPEDWINDOW, wx, wy, width, height, NULL, NULL, hinst, NULL);
    hInst = hinst;
	if (hwnd == NULL)
		return false;
	if (autoShow)
		ShowWindow(hwnd, SW_SHOW);
    if (initForGL)
        dc = GetDC(hwnd);
	exists = true;
	return true;
}

void Window::Show()
{
	if (hwnd != NULL)
		ShowWindow(hwnd, SW_SHOW);
}

void Window::Hide()
{
	if (hwnd != NULL)
		ShowWindow(hwnd, SW_HIDE);
}

const bool Window::NextMessage(MSG *msg, const bool thisWindowOnly)
{
	BOOL ret = PeekMessageA(msg, (thisWindowOnly ? hwnd : NULL), 0, 0, PM_REMOVE);
	if (msg->message == WM_DESTROY && hwnd == ParamsToHwnd(msg->wParam, msg->lParam))
	{
		exists = false;
		if (quitOnDestroy)
			PostQuitMessage(0);
		return false;
	}
	return ret > 0;
}

void Window::UnusedMessage(const MSG *msg)
{
	TranslateMessage(msg);
	DispatchMessageA(msg);
}

Window::~Window()
{
	Destroy();
}

void Window::Destroy()
{
    if (dc != NULL)
    {
        ReleaseDC(hwnd, dc);
        dc = NULL;
    }
    if (hwnd != NULL)
    {
        DestroyWindow(hwnd);
        hwnd = NULL;
        UnregisterClassA("window", hInst);
    }
	exists = false;
}

#ifdef WINMAIN_WRAPPER
#include <stdlib.h>
#ifndef ENTRYPOINT
#define ENTRYPOINT main
#endif
int _cdecl ENTRYPOINT(int argc, char **argv);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return ENTRYPOINT(__argc, __argv);
}
#endif // WINMAIN_WRAPPER
#endif // TOOLBAG_WINDOW_WIN32_IMPL_GUARD
#endif // TOOLBAG_WINDOW_WIN32_IMPL