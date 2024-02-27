#pragma once

#include "./include.h"
#include <d3d12.h>

#include "./physics/Transform.h"

//struct Transform;

class Renderer;
using namespace DirectX;

class Triangle {
public:
    Triangle();
    ~Triangle();
    void Initialize(Renderer* renderer);
    void PopulateCommandList(Renderer* renderer);
    void WaitForPreviousFrame(Renderer* renderer);
    void Render(Renderer* renderer);

private:
    // Vertex
    struct Vertex
    {
        XMFLOAT3 Pos;
        XMFLOAT4 Color;
    };


    int renderCallNum = 0;


    struct ConstantBufferData {
        XMFLOAT4X4 model;
        XMFLOAT4X4 view;
        XMFLOAT4X4 projection;
    };

    Transform m_transformData;

    ConstantBufferData m_cbData;

    ID3D12Resource* m_constantBuffer;
    UINT8* m_mappedConstantBuffer;


    // Vertex buffer
    ID3D12Resource* vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

};
