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

GameObject::GameObject(Renderer* renderer, Camera* camera, ComponentManager* componentManager) : m_cbData(), m_pRenderer(renderer), m_pCamera(camera), m_pComponentManager(componentManager) { }

void GameObject::Initialize(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) 
{
    Transform* defaultTransform = new Transform(position, rotation, scale);
    m_pComponentManager->AddComponent(*this, defaultTransform);
    
    m_cbData.model = defaultTransform->GetTransformMatrix();
    m_cbData.view = m_pCamera->GetViewMatrix();
    m_cbData.projection = m_pCamera->GetProjectionMatrix();

    MeshComponent* defaultMesh = new MeshComponent("Mesh");
    defaultMesh->Initialize(m_pRenderer);

    m_pComponentManager->AddComponent(*this, defaultTransform);
    m_pComponentManager->AddComponent(*this, defaultMesh);
}


void GameObject::Update(float deltaTime, Renderer* renderer, Camera* camera)
{
    
}

