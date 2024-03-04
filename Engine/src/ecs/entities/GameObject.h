#pragma once


#include "../../include.h"
#include "../components/Transform.h"

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Renderer;
class Component;
class Camera;

// Vertex
struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
    XMFLOAT2 Uv;
};

struct ConstantBufferData {
    XMFLOAT4X4 model;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};

class GameObject
{
public:
    GameObject();


    void Update(float deltaTime, Renderer* renderer, Camera* camera);

    //const Transform& GetTransform() const { return m_transform; }
    const ConstantBufferData& GetConstantBufferData() const { return m_cbData; }

 
    ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
    ID3D12Resource* GetIndexBuffer() const { return m_indexBuffer; }
    const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return m_vertexBufferView; }
    const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_indexBufferView; }

    std::list<Component*> componentsList;

protected:

    ConstantBufferData m_cbData;

    ID3D12Resource* m_constantBuffer;
    UINT8* m_mappedConstantBuffer;

    // Vertex buffer
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    ID3D12Resource* m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    //Transform m_transform;




private:
    void UpdateTransformation(float deltaTime);
    void UpdateDrawingOperations(Renderer* renderer);

};
