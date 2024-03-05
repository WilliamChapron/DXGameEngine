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

GameObject::GameObject() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_constantBuffer(nullptr), m_mappedConstantBuffer(nullptr), m_vertexBufferView({}), m_indexBufferView({}), m_cbData()
{
}

void GameObject::Initialize(Renderer* renderer, Camera* camera, ComponentManager* componentManager, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) {

}


void GameObject::Update(float deltaTime, Renderer* renderer, Camera* camera)
{
    
}

