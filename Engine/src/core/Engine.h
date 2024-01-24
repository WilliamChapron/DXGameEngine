// Engine.h

#pragma once

class Renderer;

#include <Windows.h>
#include <iostream>
#include "Window.h"  // Ajout de l'inclusion de l'en-tête de la classe Window

class Engine {
public:
    void Init(HINSTANCE hInstance, int nShowCmd);
    void Cleanup();

    void Run();
    void Update();

    static Engine& GetInstance() {
        static Engine instance;  // Instance unique créée statiquement
        return instance;
    }

private:
    Engine() = default;

    Renderer* m_renderer = nullptr;
    HINSTANCE m_hInstance;
    int m_nShowCmd;
    FILE* m_pConsole;
};
