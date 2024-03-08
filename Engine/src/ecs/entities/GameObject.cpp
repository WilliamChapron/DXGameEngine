#include "GameObject.hpp"

#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../../Utils.h"

#include "../components/Component.h"  
#include "../systems/ComponentManager.h"  

#include "../../core/Engine.h"  
#include "../components/Camera.h"

#include <DirectXColors.h>

#include "../components/Texture.h"
#include "../components/Transform.h"
#include "../components/Mesh.h"
#include "../components/MeshRenderer.h"
#include "../components/Collider.h"

#include "../../renderer/Resources.h"





GameObject::GameObject(ComponentManager* componentManager) : m_pComponentManager(componentManager)
{
}

void GameObject::Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale, Mesh* mesh, ConstantBufferData* cbData, Vertex* vertices, int numVertices) {
    // Specific to each object
    Transform* baseTransform = new Transform(position, rotation, scale);
    m_pComponentManager->AddComponent(*this, baseTransform);  

    MeshRenderer* baseMeshRenderer = new MeshRenderer("MeshRenderer", cbData, mesh); // Component
    baseMeshRenderer->Initialize(renderer, cbData);
    m_pComponentManager->AddComponent(*this, baseMeshRenderer);

    ColliderComponent* baseCollider = new ColliderComponent("ColliderComponent");
    baseCollider->InitializeBoundingBox(this, vertices, numVertices);
    m_pComponentManager->AddComponent(*this, baseCollider);


    //PRINT("Creation des component");
    //// Initialisation de l'objet GameObject avec un gestionnaire de composants
    //Transform* defaultTransform = new Transform(position, rotation, scale);  

    //ConstantBufferData* sendToMeshCbData = new ConstantBufferData(); 
    //sendToMeshCbData->model = defaultTransform->GetTransformMatrix();  
    //sendToMeshCbData->view = camera->GetViewMatrix(); 
    //sendToMeshCbData->projection = camera->GetProjectionMatrix();  


    //Vertex cubeVertices[] = {
    //        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
    //        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
    //        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
    //        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
    //        { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
    //        { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
    //        { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },
    //        { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }
    //};

    //UINT cubeIndices[] = {
    //    0, 1, 2,
    //    2, 1, 3,
    //    4, 6, 5,
    //    6, 7, 5,
    //    0, 2, 4,
    //    2, 6, 4,
    //    1, 5, 3,
    //    3, 5, 7,
    //    2, 3, 6,
    //    3, 7, 6,
    //    0, 4, 1,
    //    1, 4, 5
    //};

    //Vertex* pVertices = &cubeVertices[0];
    //UINT* pIndices = &cubeIndices[0];

    //int numElementsV = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
    //int numElementsI = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

    //TextureComponent* defaultTexture = new TextureComponent(textureName);
    //Mesh* defaultMesh = new Mesh(); // Class
    ////MeshRenderer* defaultMeshRenderer = new MeshRenderer("Mesh", sendToMeshCbData, defaultMesh); // Component
    //m_pMeshRenderer = new MeshRenderer("Mesh", sendToMeshCbData, defaultMesh); // Component

    //// Add to resource manager before init
    //int textureComponentID = m_pComponentManager->AddTextureToResources(defaultTexture);

    //defaultTexture->Initialize(renderer, textureComponentID);  // Initialisation du composant Texture
    //defaultMesh->Initialize(sendToMeshCbData, renderer, cubeVertices, numElementsV, cubeIndices, numElementsI);
    //m_pMeshRenderer->Initialize(renderer, sendToMeshCbData);


    //// Ajout des composants au gestionnaire de composants
    //m_pComponentManager->AddComponent(*this, defaultTransform);  
    //m_pComponentManager->AddComponent(*this, defaultTexture);  
    //m_pComponentManager->AddComponent(*this, m_pMeshRenderer);

    //PRINT("Suivant");

}







void GameObject::Update(Renderer* renderer, Camera* camera)
{
    Transform* transformComponent = GetComponent<Transform>(ComponentType::Transform);
    MeshRenderer* meshComponent = GetComponent<MeshRenderer>(ComponentType::MeshRenderer);




    ConstantBufferData* sendToMeshCbData = new ConstantBufferData();
    sendToMeshCbData->view = camera->GetViewMatrix();
    sendToMeshCbData->projection = camera->GetProjectionMatrix();


    //float rotationAngle = 0.03;
    //float rotationOffset = 0.01;
    //float translationOffset = transformComponent->GetPosition().z + 0.01f;
    //float fScale = transformComponent->GetScale().z - 0.001f;


    sendToMeshCbData->model = transformComponent->GetTransformMatrix();



    meshComponent->UpdateConstantBuffer(sendToMeshCbData);
    m_pComponentManager->UpdateComponents(this);
}