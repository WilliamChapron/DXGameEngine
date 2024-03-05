// Engine.h

#pragma once


#include <Windows.h>
#include <iostream>
#include "../ecs/systems/StateMachine.h"
class Renderer;
class GameObject;
class Window;
class GameObjectManager;
class ComponentManager;
class Camera;
class TextureComponent;
class Input;


class Engine {
public:
    void Init(HINSTANCE hInstance, int nShowCmd);
    void Cleanup();

    void Run();

    static Engine& GetInstance() {
        static Engine instance;
        return instance;
    }

    GameObject* m_pTriangle;
    GameObject* m_pTriangle2;
    GameObject* m_pTriangle3;
    Renderer* m_pRenderer;
    std::shared_ptr<GameObjectManager> m_pGameObjectManager;
    ComponentManager* m_pComponentManager;
    Camera* m_pCamera;
    Input* m_pInput;

    //std::vector<TextureComponent*> compiledTexture;

    inline void SetEngineRenderable(bool isRenderable) {
        m_isRenderable = isRenderable;
    }

    inline bool GetIsRenderable() {
        return m_isRenderable;
    }



private:

    bool m_isRenderable = false;
    Engine() = default;
    StateMachine* stateMachine;

    Window* m_pWindow = nullptr;
    HINSTANCE m_hInstance;
    int m_nShowCmd;
    FILE* m_pConsole;
};