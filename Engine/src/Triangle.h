#pragma once

#include "./include.h"
#include <d3d12.h>


using namespace DirectX;

class Triangle {
public:
    Triangle();
    ~Triangle();
    void Initialize(ID3D12Device* device);
    void Draw(ID3D12GraphicsCommandList* pCommandList);

private:
    struct Vertex {
        XMFLOAT3 position;
    };

    void CreateVertexBuffer(ID3D12Device* device, const Vertex* vertices, UINT vertexSize);

    ID3D12Resource* vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};
