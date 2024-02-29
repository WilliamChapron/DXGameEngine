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



    m_pTriangle = new Triangle;
    m_pTriangle->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

    m_pTriangle2 = new Triangle;
    m_pTriangle2->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

    m_pTriangle3 = new Triangle;
    m_pTriangle3->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(0.7f, -2.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

    m_pGameObjectManager->AddObject("Triangle", *m_pTriangle);
    m_pGameObjectManager->AddObject("Triangle2", *m_pTriangle2);
    m_pGameObjectManager->AddObject("Triangle3", *m_pTriangle3);



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
