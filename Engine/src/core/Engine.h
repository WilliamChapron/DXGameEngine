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
class ResourceManager;

class Camera;
class TextureComponent;
class Input;


class Engine {
public:
    Engine();

    void Init(HINSTANCE hInstance, int nShowCmd);
    void Cleanup();

    void Run();

    static Engine& GetInstance() {
        static Engine instance;
        //instance.m_isRenderable = false;
        return instance;
    }

    GameObject* m_pCube;
    GameObject* m_pCube2;
    GameObject* m_pCube3;
    GameObject* m_pCube4;
    Renderer* m_pRenderer;


    std::shared_ptr<GameObjectManager> m_pGameObjectManager;
    ComponentManager* m_pComponentManager;
    ResourceManager* m_pResourceManager;


    Camera* m_pCamera;
    Input* m_pInput;

    //std::vector<TextureComponent*> compiledTexture;

    inline void SetEngineRenderable(bool isRenderable) {
        m_isRenderable = isRenderable;
    }

    inline bool GetIsRenderable() {
        return m_isRenderable;
    }

    bool m_isRenderable;
private:

    //Engine() = default;
    StateMachine* stateMachine;

    Window* m_pWindow = nullptr;
    HINSTANCE m_hInstance;
    int m_nShowCmd;
    FILE* m_pConsole;
};