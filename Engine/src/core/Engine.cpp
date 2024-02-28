#include "Engine.h"
#include "../Triangle.h"
#include "../include.h"   
#include "../renderer/Graphics.h"   
#include "Defines.h"   
#include "Window.h"  

#include "../GameObject.h"
#include "../Input.h"

#include <windows.h>
#include <iostream>

using namespace DirectX;




//XMVECTOR TranslateVector(XMVECTOR vector, float deltaX, float deltaY, float deltaZ) {
//    // Cr?ation de la matrice de translation
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

    //------ TEST GAMEOBJECTM
    // Créer un objet GameObject
    GameObject gameObject;

    // Ajouter un objet
    gameObject.AddObject("1", GameObject());

    // Mettre à jour un objet
    gameObject.UpdateObject("1", GameObject());

    // Supprimer un objet
    gameObject.RemoveObject("1");

    // Méthode de rendu des objets
    gameObject.Render(nullptr);
    //------
    
    ////------ TEST INPUT
    //Input input;

    //bool running = true;
    //while (running) {
    //    PRINT("go input");
    //    input.update();
    //    PRINT("Checking input");

    //    if (input.isKeyDown(VK_ESCAPE)) {
    //        PRINT("ESCAPE pressed!");
    //        running = false;
    //    }

    //    if (input.isKeyDown(VK_UP)) {
    //        //std::cout << "UP arrow key pressed" << std::endl;
    //        PRINT("UP arrow key pressed!"); //Checking if spacebar is pressed!
    //    }

    //    if (input.isMouseDown()) {
    //        //std::cout << "Mouse left button pressed!" << std::endl;
    //        PRINT("Mouse left button pressed!"); // Check si le clic gauche est préssé
    //    }

    //    // On rajoute autant de chack inputs qu'on souhaite
    //    Sleep(100);
    //}

    ////------

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