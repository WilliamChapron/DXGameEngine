#pragma once

#include "physics/Transform.h"
#include "include.h"

using namespace DirectX;

class GameObject
{
public:
    GameObject();
    GameObject(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);

    virtual void Update(float deltaTime, Renderer* renderer);

    struct ConstantBufferData {
        XMFLOAT4X4 model;
        XMFLOAT4X4 view;
        XMFLOAT4X4 projection;
    };



    const Transform& GetTransform() const { return m_transform; }
    const int GetRenderCallNum() const { return m_renderCallNum; }
    const ConstantBufferData& GetConstantBufferData() const { return m_cbData; }

 
    ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
    ID3D12Resource* GetIndexBuffer() const { return m_indexBuffer; }
    const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return m_vertexBufferView; }
    const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_indexBufferView; }

protected:
    // Vertex
    struct Vertex
    {
        XMFLOAT3 Pos;
        XMFLOAT4 Color;
    };


    int m_renderCallNum = 0;


    Transform m_transformData;

    ConstantBufferData m_cbData;

    ID3D12Resource* m_constantBuffer;
    UINT8* m_mappedConstantBuffer;


    // Vertex buffer
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    ID3D12Resource* m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    Transform m_transform;



private:
    void UpdateTransformation(float deltaTime);
    void UpdateDrawingOperations(Renderer* renderer);

};
