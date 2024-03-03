// Engine.h

#pragma once


#include <Windows.h>
#include <iostream>
#include "../ecs/systems/StateMachine.h"
class Renderer;
class CubeMesh;
class Window;
class GameObjectManager;
class ComponentManager;
class Camera;
class TextureComponent;


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

    CubeMesh* m_pTriangle;
    CubeMesh* m_pTriangle2;
    CubeMesh* m_pTriangle3;
    Renderer* m_pRenderer;
    std::shared_ptr<GameObjectManager> m_pGameObjectManager;
    ComponentManager* m_pComponentManager;
    Camera* m_pCamera;

    std::vector<TextureComponent*> compiledTexture;

    bool isRenderable = false;

private:
    
    Engine() = default;
    StateMachine* stateMachine;

    Window* m_pWindow = nullptr;
    HINSTANCE m_hInstance;
    int m_nShowCmd;
    FILE* m_pConsole;
};