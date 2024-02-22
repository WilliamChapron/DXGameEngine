#pragma once
#include <Windows.h>
#include <iostream>

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
    // Accès à l'instance unique du singleton
    static Window& GetInstance() {
        static Window instance;  // Instance unique créée statiquement
        return instance;
    }

    // Fonction d'initialisation
    bool Init(HINSTANCE hInstance, int nShowCmd);

    // Fonction de gestion des messages de la fenêtre
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    inline WindowProperties getWndProps() {
        return m_wndProps;
    }                      

private:
    // Constructeur privé pour empêcher la création d'instances supplémentaires
    Window() = default;

    WindowProperties m_wndProps;
};
