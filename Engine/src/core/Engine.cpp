#include "Engine.h"
#include "../Triangle.h"
#include "../include.h"   
#include "../renderer/Renderer.h"   
#include "Defines.h"   

using namespace DirectX;




XMVECTOR TranslateVector(XMVECTOR vector, float deltaX, float deltaY, float deltaZ) {
    // Création de la matrice de translation
    XMMATRIX translationMatrix = XMMatrixTranslation(deltaX, deltaY, deltaZ);

    // Application de la translation au vecteur
    return XMVector3Transform(vector, translationMatrix);
}


void Engine::Init(HINSTANCE hInstance, int nShowCmd) {
    m_hInstance = hInstance;

    AllocConsole();

    if (freopen_s(&m_pConsole, "CONOUT$", "w", stdout) != 0) {
        MessageBox(0, L"Failed to redirect console output", L"Error", MB_OK);
        Cleanup();
        PostQuitMessage(1);
    }

    Window::GetInstance().Init(hInstance, nShowCmd);


    m_pRenderer = new Renderer();


    m_pRenderer->InitializeDirectX12Instances();

    m_pTriangle = new Triangle;
    m_pTriangle->Initialize(m_pRenderer);


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
            m_pTriangle->Render(m_pRenderer);
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
