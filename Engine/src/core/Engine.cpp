#include "Engine.h"
#include "../include.h"   
#include "../renderer/Graphics.h"   
#include "Defines.h"   
#include "Window.h"  

// System
#include "../ecs/systems/GameObjectManager.h"  
#include "../ecs/systems/ComponentManager.h"  


// Ent
#include "../ecs/entities/GameObject.hpp"          

// Component
#include "../ecs/components/Transform.h"
#include "../ecs/components/Texture.h"
#include "../ecs/components/Camera.h"
#include "../ecs/components/Mesh.h"

#include "../ecs/components/MeshRenderer.h"

// Miscellaneous
#include "../ecs/systems/Time.h"
#include "../Utils.h"
#include "../ecs/systems/Input.h"




using namespace DirectX;


Engine::Engine()
{

}

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

    // INITIALIZE UNIQUE COMPONENT

    Vertex cubeVertices[] = {
            { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
            { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
            { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },
            { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }
    };

    UINT cubeIndices[] = {
        0, 1, 2,
        2, 1, 3,
        4, 6, 5,
        6, 7, 5,
        0, 2, 4,
        2, 6, 4,
        1, 5, 3,
        3, 5, 7,
        2, 3, 6,
        3, 7, 6,
        0, 4, 1,
        1, 4, 5
    };

    Vertex* pVertices = &cubeVertices[0];
    UINT* pIndices = &cubeIndices[0];

    int numElementsV = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
    int numElementsI = sizeof(cubeIndices) / sizeof(cubeIndices[0]);



    ConstantBufferData* cbData = new ConstantBufferData(); // Alloue de la mémoire pour m_cbData

    XMStoreFloat4x4(&cbData->model, XMMatrixIdentity()); ;
    cbData->view = m_pCamera->GetViewMatrix();
    cbData->projection = m_pCamera->GetProjectionMatrix();

    TextureComponent* texture = new TextureComponent("texture");
    TextureComponent* texture2 = new TextureComponent("texture2");
    int textureComponentID = m_pComponentManager->AddTextureToResources(texture);
    PRINT("Texture Component 1 ID: " + std::to_string(textureComponentID));
    int textureComponentID2 = m_pComponentManager->AddTextureToResources(texture2);
    PRINT("Texture Component 2 ID: " + std::to_string(textureComponentID));



    Mesh* defaultMesh = new Mesh(); // Class || INITIALIZE MESH BEFORE TEXTURE BECAUSE DRAWING ORDER !!!






    texture->Initialize(m_pRenderer, textureComponentID);  
    texture2->Initialize(m_pRenderer, textureComponentID2); 
    defaultMesh->Initialize(cbData, m_pRenderer, cubeVertices, numElementsV, cubeIndices, numElementsI);



    //Transform* transform1 = new Transform(XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
    //Transform* transform2 = new Transform(XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
    //Transform* transform3 = new Transform(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
    //Transform* transform4 = new Transform(XMFLOAT3(1.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

    //Create Objects
    m_pCube = new GameObject(m_pComponentManager);
    m_pCube2 = new GameObject(m_pComponentManager);
    m_pCube3 = new GameObject(m_pComponentManager);
    m_pCube4 = new GameObject(m_pComponentManager);

    m_pComponentManager->AddComponent(*m_pCube, texture);  
    m_pComponentManager->AddComponent(*m_pCube2, texture2); 
    m_pComponentManager->AddComponent(*m_pCube3, texture);  

    m_pComponentManager->AddComponent(*m_pCube4, texture2);  

    m_pCube->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(-0.9f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), defaultMesh, cbData);
    m_pCube2->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), defaultMesh, cbData);
    m_pCube3->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), defaultMesh, cbData);
    m_pCube4->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(1.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), defaultMesh, cbData);








    m_pGameObjectManager->AddObject("Cube", m_pCube);
    //m_pGameObjectManager->AddObject("Cube2", m_pCube2);
    //m_pGameObjectManager->AddObject("Cube3", m_pCube3);
    m_pGameObjectManager->AddObject("Cube4", m_pCube4);


    //std::map<int, TextureComponent*> theArray = m_pComponentManager->GetTextureComponents();
    //for (const auto& pair : theArray) {
    //    std::cout << "Component ID: " << pair.first << std::endl;
    //    std::cout << "Texture: " << pair.second->GetName() << std::endl;
    //}


    // Drawing
    m_isRenderable = true;
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

        Transform* transformComponent = m_pCube->GetComponent<Transform>(ComponentType::Transform);
        float translationOffset = transformComponent->GetPosition().y + 0.1f;
        transformComponent->SetPosition(0, translationOffset, 0);

        time.UpdateTime();

        //CAMERA DEBUG
        //m_pCamera->UpdatePosition(0.0f, 0.0f, 0.2f);
        //m_pCamera->Rotate(0.0f, 0.0f, 0.25f);
        /*m_pCamera->RotateAroundTarget(0.f, .0f, 0.2f);
        m_pCamera->UpdateTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));*/
        //m_pCamera->Rotate(m_pInput->GetMousePosition().x, m_pInput->GetMousePosition().y, 0.f);

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
        //PRINT(m_isRenderable);
            // Appelez la fonction Render de la classe Renderer et passez-lui la liste de Cubes
        m_pGameObjectManager->Update(m_pRenderer);
    }
}

