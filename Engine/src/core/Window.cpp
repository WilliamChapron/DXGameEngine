#include "Window.h"
#include "Engine.h"
#include "Defines.h"   

Window::Window(HINSTANCE hInstance, int nShowCmd) {


    /*
    Store Window Data
    */

    m_wndProps.hwnd = nullptr;
    m_wndProps.windowName = L"GameEngineDX12App";
    m_wndProps.windowTitle = L"GameEngineDX12";
    m_wndProps.width = 1200;
    m_wndProps.height = 900;
    m_wndProps.fullScreen = false;

    /*
    Set up and Create Window
    */

    if (m_wndProps.fullScreen) {
        HMONITOR hmon = MonitorFromWindow(m_wndProps.hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfo(hmon, &mi);
        m_wndProps.width = mi.rcMonitor.right - mi.rcMonitor.left;
        m_wndProps.height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    }

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &Window::WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_wndProps.windowName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Erreur d'enregistrement de la classe", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    m_wndProps.hwnd = CreateWindowEx(NULL, m_wndProps.windowName, m_wndProps.windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, hInstance, NULL);

    if (!m_wndProps.hwnd) {
        MessageBox(NULL, L"Erreur de création de la fenêtre", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    if (m_wndProps.fullScreen) {
        SetWindowLong(m_wndProps.hwnd, GWL_STYLE, 0);
    }

    /*
    Show and Update one time window
    */

    ShowWindow(m_wndProps.hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_wndProps.hwnd);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /*
    Switch event Window
    */

    Engine& engineInstance = Engine::GetInstance();

    switch (message)
    {
    case WM_CREATE:
    {
        
    }
    return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            if (MessageBox(0, L"Êtes-vous sûr de vouloir quitter ?", L"Vraiment ?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
        }
        return 0;

    case WM_KEYUP:
        return 0;

    case WM_PAINT:
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }


    return DefWindowProc(hwnd, message, wParam, lParam);
}

void Window::UpdateTitleWithFPS(int fps)
{
    std::string chain = "DirectX12 Engine FPS : " + std::to_string(fps);

    std::wstring newTitle(chain.begin(), chain.end());

    SetWindowText(m_wndProps.hwnd, newTitle.c_str());
}

