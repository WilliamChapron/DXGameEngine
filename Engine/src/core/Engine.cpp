#include "Engine.h"
#include "../ecs/entities/Triangle.h"
#include "../include.h"   
#include "../renderer/Graphics.h"   
#include "Defines.h"   
#include "Window.h"  
#include "../ecs/systems/GameObjectManager.h"  

#include "../Utils.h"
#include "../ecs/components/Transform.h"

#include "../ecs/systems/Time.h"


#include "../ecs/components/Camera.h"

#include "../ecs/systems/Input.h"

using namespace DirectX;




//XMVECTOR TranslateVector(XMVECTOR vector, float deltaX, float deltaY, float deltaZ) {
//    // Création de la matrice de translation
//    XMMATRIX translationMatrix = XMMatrixTranslation(deltaX, deltaY, deltaZ);
//
//    // Application de la translation au vecteur
//    return XMVector3Transform(vector, translationMatrix);
//}


void Engine::Init(HINSTANCE hInstance, int nShowCmd) {
    m_hInstance = hInstance;

    AllocConsole();

    if (freopen_s(&m_pConsole, "CONOUT$", "w", stdout) != 0) {
        MessageBox(0, L"Failed to redirect console output", L"Error", MB_OK);
        Cleanup();
        PostQuitMessage(1);
    }

    m_pWindow = new Window(hInstance, nShowCmd);


    m_pRenderer = new Renderer(m_pWindow);

    m_pRenderer->InitializeDirectX12Instances();

    m_pGameObjectManager = new GameObjectManager();

    m_pCamera = new Camera();
    
    m_pStateMachine = new StateMachine();

    m_pInput = new Input();



    m_pTriangle = new Triangle;
    m_pTriangle->Initialize(m_pRenderer, m_pCamera);

    m_pGameObjectManager->AddObject("Triangle", *m_pTriangle);



    isRenderable = true;
    Run();
}

void Engine::Cleanup() {
    if (m_pConsole) {
        fclose(m_pConsole);
        FreeConsole();
    }
}

void Engine::Run() {
    std::cout << "Main Loop Started" << std::endl;

    Time time;

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));


    std::vector<GameObject> triangles;


    //triangles.push_back(*m_pTriangle1);
    //triangles.push_back(*m_pTriangle2);
    // Ajoutez d'autres triangles au besoin

    while (true) {
        time.UpdateTime();
        m_pWindow->UpdateTitleWithFPS(time.GetFramePerSecond());

        //----- TEST INPUT
        m_pInput->Update();

        // Affichez la liste des touches et leur état
        std::cout << "Touches pressees : " << std::endl;
        for (const auto& pair : m_pInput->GetKeyStates()) {
            std::cout << "Touche : " << pair.first << ", Etat : ";
            // Utilisez un switch pour gérer les différents états de la touche
            switch (pair.second) {
            case KeyState::Pressed:
                std::cout << "Pressed";
                break;
            case KeyState::Held:
                std::cout << "Held";
                break;
            case KeyState::Released:
                std::cout << "Released";
                break;
            case KeyState::Inactive:
                std::cout << "Inactive";
                break;
            }
            std::cout << std::endl;
        }
        // Affichez les coordonnées de la souris
        std::cout << "Position de la souris : X = " << m_pInput->GetMousePosition().x << ", Y = " << m_pInput->GetMousePosition().y << std::endl;

        //-----

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (isRenderable) {
            // Appelez la fonction Render de la classe Renderer et passez-lui la liste de triangles
            m_pGameObjectManager->Update(m_pRenderer);
        }
    }
}

void Engine::Update() {
    // static instance

}
