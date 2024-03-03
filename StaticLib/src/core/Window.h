#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

struct WindowProperties
{
    HWND hwnd;
    LPCTSTR windowName;
    LPCTSTR windowTitle;
    int width;
    int height;
    bool fullScreen;
};

class Window {
public:
    Window(HINSTANCE hInstance, int nShowCmd);
    ~Window();

    void UpdateTitleWithFPS(int fps);

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    inline WindowProperties getWndProps() {
        return m_wndProps;
    }

private:
    WindowProperties m_wndProps;
};

