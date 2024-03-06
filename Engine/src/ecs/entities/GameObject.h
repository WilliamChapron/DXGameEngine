#pragma once


#include "../../include.h"
#include "../components/Transform.h"



#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>

#include "../systems/ComponentManager.h"


using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Renderer;
class Component;
class Camera;

struct ConstantBufferData {
    XMFLOAT4X4 model;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};

class GameObject
{
public:
    GameObject(ComponentManager* componentManager);

    void Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale, std::string textureName);
    void Update(Renderer* renderer, Camera* camera);

    //const Transform& GetTransform() const { return m_transform; }
    const ConstantBufferData& GetConstantBufferData() const { return m_cbData; }

    template <typename T>
    T* GetComponent(ComponentType type);

    std::list<Component*> componentsList;

protected:
    
private:

    ConstantBufferData m_cbData;

    ComponentManager* m_pComponentManager;
};

template <typename T>
T* GameObject::GetComponent(ComponentType type) {
    Component* component = m_pComponentManager->GetComponentByType(*this, type);
    return dynamic_cast<T*>(component);
}
