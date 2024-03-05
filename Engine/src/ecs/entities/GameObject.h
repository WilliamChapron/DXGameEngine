#pragma once


#include "../../include.h"
#include "../components/Transform.h"

#include "../systems/ComponentManager.h"

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
class ComponentManager;
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

    void Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);
    void Update(Renderer* renderer, Camera* camera);

    //const Transform& GetTransform() const { return m_transform; }
    const ConstantBufferData& GetConstantBufferData() const { return m_cbData; }


    ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
    ID3D12Resource* GetIndexBuffer() const { return m_indexBuffer; }


    template <typename T>
    T* GetComponent(ComponentType type);

    const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return m_vertexBufferView; }
    const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_indexBufferView; }

    std::list<Component*> componentsList;


protected:
    
private:

    ConstantBufferData m_cbData;

    ID3D12Resource* m_constantBuffer;
    UINT8* m_mappedConstantBuffer;

    // Vertex buffer
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    ID3D12Resource* m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

    ComponentManager* m_pComponentManager;
    //Transform m_transform;

};