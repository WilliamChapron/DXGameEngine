#include "Engine.h"
#include "../Triangle.h"
#include "../include.h"   
#include "../renderer/Graphics.h"   
#include "Defines.h"   
#include "Window.h"  

#include "../Utils.h"
#include "../physics/Transform.h"

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





            // Créer une instance de la classe Transform
        Transform transform;

        // Déboguer la matrice de transformation initiale
        XMFLOAT4X4 initialTransformMatrix = transform.GetTransformMatrix();
        std::cout << "Initial Transform Matrix:" << std::endl;
        PrintMatrix(initialTransformMatrix);

        // Effectuer une rotation relative pour mettre à jour la transformation
        transform.Rotate(45.0f, 0.0f, 0.0f);

        // Déboguer la matrice de transformation mise à jour
        XMFLOAT4X4 updatedTransformMatrix = transform.GetTransformMatrix();
        std::cout << "Updated Transform Matrix:" << std::endl;
        PrintMatrix(updatedTransformMatrix);
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
