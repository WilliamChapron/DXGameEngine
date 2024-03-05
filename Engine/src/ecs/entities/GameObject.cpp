#include "GameObject.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../../Utils.h"

#include "../components/Component.h"  
#include "../systems/ComponentManager.h"  

#include "../../core/Engine.h"  
#include "../components/Camera.h"


#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include <stdexcept>

#include "../../Utils.h"
#include <DirectXColors.h>

#include "../components/Camera.h"
#include "../components/Texture.h"
#include "../components/Transform.h"
#include "../components/Mesh.h"

#include "../../renderer/Resources.h"



#include <iostream>
#include <iomanip>

GameObject::GameObject(ComponentManager* componentManager) : m_cbData() , m_pComponentManager(componentManager)
{
}

void GameObject::Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)  {
    //m_transform = Transform(position, rotation, scale);
    //m_cbData.model = m_transform.GetTransformMatrix();



    Transform* defaultTransform = new Transform(position, rotation, scale);

    // Matrices
    m_cbData.model = defaultTransform->GetTransformMatrix();
    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();


    // Initialisation des meshes, textures et shader
    MeshComponent* defaultMesh = new MeshComponent("Mesh", &m_cbData);
    PRINT("Mesh import");
    TextureComponent* defaultTexture = new TextureComponent("Texture");
    //ShaderComponent* defaultShader = new ShaderComponent("Shader");

    defaultMesh->Initialize(renderer);
    PRINT("Mesh Init ok");
    defaultTexture->Initialize(renderer);
    //defaultShader->Initialize(renderer);


    m_pComponentManager->AddComponent(*this, defaultTransform);
    m_pComponentManager->AddComponent(*this, defaultMesh);
    m_pComponentManager->AddComponent(*this, defaultTexture);
    //componentManager->AddComponent(*this, defaultShader);

    //defaultTransform->Update();



}


void GameObject::Update(Renderer* renderer, Camera* camera)
{
    Engine& e = Engine::GetInstance();
    Component* component = e.m_pComponentManager->GetComponentByType(*this, ComponentType::Transform);
    Transform* transformComponent = dynamic_cast<Transform*>(component);

    std::cout << "Update" << std::endl;


    float rotationAngle = 0.03;
    float rotationOffset = 0.01;
    //PRINT("Translation offset");
    //PRINT(m_transform.vPosition.z);
    float translationOffset = transformComponent->GetPosition().z + 0.01f;
    float fScale = transformComponent->GetScale().z - 0.001f;
    //printFloatWithPrecision(m_transform.vScale.z, 4);


    //m_transform.Translate(m_transform.vPosition.x, m_transform.vPosition.y, translationOffset);
    transformComponent->Rotate(0, 0, rotationAngle);
    //m_transform.Scale(fScale, fScale, fScale);


    m_cbData.model = transformComponent->GetTransformMatrix();

    m_pComponentManager->UpdateComponents(this);


    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();



}

