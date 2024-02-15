#pragma once

#include "./include.h"
#include <d3d12.h>

class Renderer;

using namespace DirectX;

class Triangle {
public:
    Triangle();
    ~Triangle();
    void Initialize(Renderer* renderer);
    void PopulateCommandList(Renderer* renderer);
    void WaitForPreviousFrame(Renderer* renderer);
    void Draw(Renderer* renderer);

private:
    struct Vertex {
        XMFLOAT3 position;
    };

    ID3DBlob* vertexShaderBlob;
    ID3DBlob* pixelShaderBlob;

    ID3D12Resource* vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissorRect;  
};
