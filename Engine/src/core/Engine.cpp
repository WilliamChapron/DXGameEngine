#include "Engine.h"
#include "../include.h"   
#include "../renderer/Graphics.h"   
#include "Defines.h"   
#include "Window.h"  

// System
#include "../ecs/systems/GameObjectManager.h"  
#include "../ecs/systems/ComponentManager.h"  
#include "../ecs/systems/ResourceManager.h"


// Ent
#include "../ecs/entities/GameObject.hpp"          

// Component
#include "../ecs/components/Transform.h"
#include "../ecs/components/Texture.h"
#include "../ecs/components/Camera.h"
#include "../ecs/components/Mesh.h"
#include "../ecs/components/Shader.h"

#include "../ecs/components/MeshRenderer.h"

// Miscellaneous
#include "../ecs/systems/Time.h"
#include "../Utils.h"
#include "../ecs/systems/Input.h"


#include "../CommonsMesh.h"

using namespace DirectX;




Engine::Engine()
{

}



void Engine::Init(HINSTANCE hInstance, int nShowCmd) {
    CubeMesh cubeMesh; // Common Mesh
    TriangleMesh cubeMesh2; // Common Mesh

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
    m_pResourceManager = new ResourceManager();

    // INITIALIZE UNIQUE COMPONENT

    

    ConstantBufferData* cbData = new ConstantBufferData(); // Alloue de la mémoire pour m_cbData
    XMStoreFloat4x4(&cbData->model, XMMatrixIdentity()); ;
    cbData->view = m_pCamera->GetViewMatrix();
    cbData->projection = m_pCamera->GetProjectionMatrix();

    TextureComponent* texture = new TextureComponent("texture");
    TextureComponent* texture2 = new TextureComponent("texture2");

    Mesh* baseMesh = new Mesh("mesh1");
    Mesh* baseMesh2 = new Mesh("mesh2");

    ShaderComponent* baseShader = new ShaderComponent("shader1", m_pRenderer);

    m_pResourceManager->AddTextureToResources(texture);
    m_pResourceManager->AddTextureToResources(texture2);
    m_pResourceManager->AddMeshToResources(baseMesh);
    m_pResourceManager->AddMeshToResources(baseMesh2);

    m_pResourceManager->AddShaderToResources(baseShader);


    // Any order
    baseMesh->Initialize(cbData, m_pRenderer, cubeMesh.cubeVertices, cubeMesh.numElementsV, cubeMesh.cubeIndices, cubeMesh.numElementsI);
    baseMesh2->Initialize(cbData, m_pRenderer, cubeMesh2.cubeVertices, cubeMesh2.numElementsV, cubeMesh2.cubeIndices, cubeMesh2.numElementsI);

    texture->Initialize(m_pRenderer, m_pResourceManager->FindTextureComponentByName("texture").key);
    texture2->Initialize(m_pRenderer, m_pResourceManager->FindTextureComponentByName("texture2").key);

    baseShader->InitializeRootSignature();
    baseShader->InitializePSO();


    // #TODO CREATE FONCTION | CREATE OBJECT | CREATE TEXTURE | CREATE MESH | CREATE SHADER
    m_pCube = new GameObject(m_pComponentManager);
    m_pCube2 = new GameObject(m_pComponentManager);
    m_pCube3 = new GameObject(m_pComponentManager);
    m_pCube4 = new GameObject(m_pComponentManager);



    m_pCube->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(-1.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), m_pResourceManager->FindMeshComponentByName("mesh1").component, cbData, cubeMesh.cubeVertices, cubeMesh.numElementsV);
    m_pCube2->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), m_pResourceManager->FindMeshComponentByName("mesh2").component, cbData, cubeMesh2.cubeVertices, cubeMesh2.numElementsV);
    m_pCube3->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(1.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), m_pResourceManager->FindMeshComponentByName("mesh1").component, cbData, cubeMesh.cubeVertices, cubeMesh.numElementsV);
    m_pCube4->Initialize(m_pRenderer, m_pCamera, XMFLOAT3(3.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), m_pResourceManager->FindMeshComponentByName("mesh2").component, cbData, cubeMesh2.cubeVertices, cubeMesh2.numElementsV);

    m_pComponentManager->AddComponent(*m_pCube, m_pResourceManager->FindTextureComponentByName("texture").component);
    m_pComponentManager->AddComponent(*m_pCube2, m_pResourceManager->FindTextureComponentByName("texture2").component);
    m_pComponentManager->AddComponent(*m_pCube3, m_pResourceManager->FindTextureComponentByName("texture").component);
    m_pComponentManager->AddComponent(*m_pCube4, m_pResourceManager->FindTextureComponentByName("texture2").component);

    m_pComponentManager->AddComponent(*m_pCube, m_pResourceManager->FindShaderComponentByName("shader1").component);
    m_pComponentManager->AddComponent(*m_pCube2, m_pResourceManager->FindShaderComponentByName("shader1").component);
    m_pComponentManager->AddComponent(*m_pCube3, m_pResourceManager->FindShaderComponentByName("shader1").component);
    m_pComponentManager->AddComponent(*m_pCube4, m_pResourceManager->FindShaderComponentByName("shader1").component);

    m_pGameObjectManager->AddObject("Cube", m_pCube);
    //m_pGameObjectManager->AddObject("Cube2", m_pCube2);
    m_pGameObjectManager->AddObject("Cube3", m_pCube3);
    //m_pGameObjectManager->AddObject("Cube4", m_pCube4);



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
        float translationOffset = transformComponent->GetPosition().x + 0.008f;
        transformComponent->SetPosition(translationOffset, 0, 0);

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
        //std::cout << "mouse x : "
        //    << m_pInput->GetMousePosition().x
        //    << "   mouse y : "
        //    << m_pInput->GetMousePosition().y
        //    << std::endl;

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

