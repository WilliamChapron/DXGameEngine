// Engine.h

#pragma once


#include <Windows.h>
#include <iostream>

class Renderer;
class Triangle;
class Window;



class Engine {
public:
    void Init(HINSTANCE hInstance, int nShowCmd);
    void Cleanup();

    void Run();
    void Update();

    static Engine& GetInstance() {
        static Engine instance;  
        return instance;
    }

    Triangle* m_pTriangle;
    Renderer* m_pRenderer;

    bool isRenderable = false;

private:
    Engine() = default;

    Window* m_pWindow = nullptr;
    HINSTANCE m_hInstance;
    int m_nShowCmd;
    FILE* m_pConsole;
};
