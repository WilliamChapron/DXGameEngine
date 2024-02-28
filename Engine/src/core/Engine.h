// Engine.h

#pragma once


#include <Windows.h>
#include <iostream>
#include "../ecs/systems/StateMachine.h"
class Renderer;
class Triangle;
class Window;
class GameObjectManager;

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
    GameObjectManager* m_pGameObjectManager;

    bool isRenderable = false;

private:
    
    Engine() = default;
    StateMachine* stateMachine;

    Window* m_pWindow = nullptr;
    HINSTANCE m_hInstance;
    int m_nShowCmd;
    FILE* m_pConsole;
};