#include "Engine.h"
#include <wrl/client.h>
#include "../include.h"   
#include "../renderer/Renderer.h"   

using namespace DirectX;




XMVECTOR TranslateVector(XMVECTOR vector, float deltaX, float deltaY, float deltaZ) {
    // Création de la matrice de translation
    XMMATRIX translationMatrix = XMMatrixTranslation(deltaX, deltaY, deltaZ);

    // Application de la translation au vecteur
    return XMVector3Transform(vector, translationMatrix);
}


void Engine::Init(HINSTANCE hInstance, int nShowCmd) {
    m_hInstance = hInstance;

    // Créer la console
    AllocConsole();

    if (freopen_s(&m_pConsole, "CONOUT$", "w", stdout) != 0) {
        MessageBox(0, L"Failed to redirect console output", L"Error", MB_OK);
        Cleanup();
        PostQuitMessage(1);
    }

    // Initialiser la fenêtre
    Window::GetInstance().Init(hInstance, nShowCmd);


    m_renderer = new Renderer();
    // Initialiser les ressources DirectX 12
    m_renderer->InitializeDirectX12Instances();

    //// Création d'un vecteur représentant la position initiale
    //XMVECTOR initialPosition = XMVectorSet(1.0f, 2.0f, 3.0f, 1.0f);

    //// Affichage de la position initiale
    //std::cout << "Position initiale : (" << XMVectorGetX(initialPosition) << ", " << XMVectorGetY(initialPosition) << ", " << XMVectorGetZ(initialPosition) << ")\n";

    //// Translation du vecteur
    //XMVECTOR newPosition = TranslateVector(initialPosition, 1.0f, 2.0f, -3.0f);

    //// Affichage de la nouvelle position
    //std::cout << "Nouvelle position : (" << XMVectorGetX(newPosition) << ", " << XMVectorGetY(newPosition) << ", " << XMVectorGetZ(newPosition) << ")\n";

    // Lancer la boucle principale
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
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //else {
        //    Sleep(1000);
        //    std::cout << "sdsd" << std::endl;
        //}
    }
}
void Engine::Update() {
    // static instance

}
