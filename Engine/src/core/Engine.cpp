#include "Engine.h"
//#include "../Triangle.h"
#include "../GameObject.h"
#include "../include.h"   
#include "../renderer/Graphics.h"   
#include "Defines.h"   
#include "Window.h"  


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

    /*m_pTriangle = new Triangle;
    m_pTriangle->Initialize(m_pRenderer);*/

    m_pGameObject = new GameObject;
    m_pGameObject->Initialize(m_pRenderer);
    Triangle triangle1 = { { {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} }, {1.0f, 0.0f, 0.0f, 1.0f} };
    m_pGameObject->AddTriangle(triangle1);

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


    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (true) {
        //PRINT("Hello");
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (isRenderable) {
            //PRINT("Paint");
            //m_pTriangle->Render(m_pRenderer);
            m_pGameObject->Render(m_pRenderer);
        }
        //m_pTriangle->Render(m_pRenderer);
        //else {
        //    Sleep(1000);
        //    std::cout << "sdsd" << std::endl;
        //}
    }
}
void Engine::Update() {
    // static instance

}
