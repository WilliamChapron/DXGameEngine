#include "Engine.h"
#include "../ecs/entities/GameObject.h"
#include "../include.h"   
#include "../renderer/Graphics.h"   
#include "Defines.h"   
#include "Window.h"  

// System
#include "../ecs/systems/GameObjectManager.h"  
#include "../ecs/systems/ComponentManager.h"  



// Component
#include "../ecs/components/Transform.h"
#include "../ecs/components/Texture.h"
#include "../ecs/components/Camera.h"


// Miscellaneous
#include "../ecs/systems/Time.h"
#include "../Utils.h"
#include "../ecs/systems/Input.h"




using namespace DirectX;


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
    m_pRenderer->m_pCommandList->Close();

    m_pInput = new Input(m_pWindow->getWndProps());
    m_pInput->ResetMousePosition();


    m_pCamera = new Camera(); // #TODO Shared ptr camera to each object



    m_pGameObjectManager = std::make_shared<GameObjectManager>(m_pCamera);
    m_pComponentManager = new ComponentManager(m_pGameObjectManager, m_pRenderer, m_pCamera);

    //Create Objects
    m_pCube = new GameObject(m_pComponentManager);
    //m_pCube2 = new GameObject(m_pComponentManager);

    m_pCube->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(0.f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), "texture");
    //m_pCube2->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), "texture2");


    m_pGameObjectManager->AddObject("Cube", m_pCube);
    //m_pGameObjectManager->AddObject("Cube2", m_pCube2);

    // Drawing
    SetEngineRenderable(true);
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


    std::vector<GameObject> Cubes;

    float speed = 15.f;

    ShowCursor(FALSE);

    m_pCamera->UpdateTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));
    //Cubes.push_back(*m_pCube1);
    //Cubes.push_back(*m_pCube2);
    // Ajoutez d'autres Cubes au besoin

    while (true) {

        time.UpdateTime();

        m_pCamera->Update(time.GetDeltaTime());

        m_pWindow->UpdateTitleWithFPS(time.GetFramePerSecond());

        //------ TEST INPUT

        //// Affichez la liste des touches et leur �tat
        //std::cout << "Touches pressees : " << std::endl;
        //for (const auto& pair : m_pInput->GetKeyStates()) {
        //    std::cout << "Touche : " << pair.first << ", Etat : ";
        //    // Utilisez un switch pour g�rer les diff�rents �tats de la touche
        //    switch (pair.second) {
        //    case KeyState::Pressed:
        //        std::cout << "Pressed";
        //        break;
        //    case KeyState::Held:
        //        std::cout << "Held";
        //        break;
        //    case KeyState::Released:
        //        std::cout << "Released";
        //        break;
        //    case KeyState::Inactive:
        //        std::cout << "Inactive";
        //        break;
        //    }
        //    std::cout << std::endl;
        //}


        m_pInput->Update();

        //------ Camera Movement
        for (const auto& pair : m_pInput->GetKeyStates()) {
            switch (pair.first) {
                case 'Z':
                    if (pair.second == KeyState::Pressed || pair.second == KeyState::Held)
                        m_pCamera->UpdatePosition(0.0f, speed * time.GetDeltaTime(), 0.0f);
                    break;
                case 'S':
                    if (pair.second == KeyState::Pressed || pair.second == KeyState::Held)
                        m_pCamera->UpdatePosition(0.0f, -speed * time.GetDeltaTime(), 0.0f);
                    break;
                case 'Q':
                    if (pair.second == KeyState::Pressed || pair.second == KeyState::Held)
                        m_pCamera->UpdatePosition(-speed * time.GetDeltaTime(), 0.0f, 0.0f);
                    break;
                case 'D':
                    if (pair.second == KeyState::Pressed || pair.second == KeyState::Held)
                        m_pCamera->UpdatePosition(speed * time.GetDeltaTime(), 0.0f, 0.0f);
                    break;
                case VK_SPACE:
                    if (pair.second == KeyState::Pressed || pair.second == KeyState::Held)
                        m_pCamera->UpdatePosition(0.0f, 0.0f, -speed * time.GetDeltaTime());
                    break;
                case VK_SHIFT:
                    if (pair.second == KeyState::Pressed || pair.second == KeyState::Held)
                        m_pCamera->UpdatePosition(0.0f, 0.0f, speed * time.GetDeltaTime());
                    break;
                /*case VK_LBUTTON:
                    if (pair.second == KeyState::Pressed || pair.second == KeyState::Held)
                    {
                        
                    }*/
            }
        }
        m_pCamera->Rotate(m_pInput->GetMousePosition().y * 0.2f, 0.0f, - m_pInput->GetMousePosition().x * 0.2f);
        std::cout << "mouse x : "
            << m_pInput->GetMousePosition().x
            << "   mouse y : "
            << m_pInput->GetMousePosition().y
            << std::endl;

        m_pInput->ResetMousePosition();

        /***
        * J'ai un problème par rapport à la rotate de la caméra
        * Lorsque je rotate ma caméra, elle rotate par rapport au nouvel axe
        * MAIS mes déplacement restent sur l'axe originel, faudrait que j'arrive à update le nouvel axe     
        ***/
        
        //------ Camera Movement
        



        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (GetIsRenderable()) {
            // Appelez la fonction Render de la classe Renderer et passez-lui la liste de Cubes
            m_pGameObjectManager->Update(m_pRenderer, m_pCamera);
        }
    }
}

