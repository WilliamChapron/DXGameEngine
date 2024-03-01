

#include "Triangle.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include <stdexcept>

#include "../../Utils.h"
#include <DirectXColors.h>

#include "../components/Camera.h"

#include "../../renderer/Resources.h"
#include "../../DDSTextureLoader.h"





Triangle::Triangle() : GameObject()
{
}

Triangle::~Triangle() {

    if (m_vertexBuffer != nullptr) {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }
}


void Triangle::Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) {
    m_transform = Transform(position, rotation, scale);
    m_cbData.model = m_transform.GetTransformMatrix();

    // ** MATRIX

    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();

    // MATRIX **
    HRESULT hr;
    hr = renderer->m_pCommandAllocator->Reset();
    ASSERT_FAILED(hr);

    hr = renderer->m_pCommandList->Reset(renderer->m_pCommandAllocator.Get(), nullptr);
    ASSERT_FAILED(hr);

    Vertex cubeVertices[] = {
        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  // Rouge
        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  // Rouge
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },  // Bleu
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },  // Bleu
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  // Jaune
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  // Jaune
        { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },  // Gris
        { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }   // Gris
    };

    UINT cubeIndices[] = {
        0, 1, 2,   // Face 1 (clockwise)
        2, 1, 3,   // Face 2 (clockwise)
        4, 6, 5,   // Face 3 (clockwise)
        6, 7, 5,   // Face 4 (clockwise)
        0, 2, 4,   // Face 5 (clockwise)
        2, 6, 4,   // Face 6 (clockwise)
        1, 5, 3,   // Face 7 (clockwise)
        3, 5, 7,   // Face 8 (clockwise)
        2, 3, 6,   // Face 9 (clockwise)
        3, 7, 6,   // Face 10 (clockwise)
        0, 4, 1,   // Face 11 (clockwise)
        1, 4, 5    // Face 12 (clockwise)
    };

    const UINT vertexBufferSize = sizeof(cubeVertices);// *sizeof(Vertex);
    const UINT indexBufferSize = sizeof(cubeIndices);// *sizeof(UINT);
    const UINT stride = sizeof(Vertex);


    CreateIndexBuffer(indexBufferSize, cubeIndices, m_indexBuffer, m_indexBufferView, renderer);
    CreateVertexBuffer(vertexBufferSize, cubeVertices, m_vertexBuffer, m_vertexBufferView, stride, renderer);

    // * Constant Buffer
    CD3DX12_HEAP_PROPERTIES cbHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC cbDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstantBufferData) + 255) & ~255);

    hr = renderer->m_pDevice->CreateCommittedResource(
        &cbHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &cbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)
    );
    ASSERT_FAILED(hr);

    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);


    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = (sizeof(ConstantBufferData) + 255) & ~255; // Alignement sur 256 octets
    renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
    // Constant Buffer *





    hr = CreateDDSTextureFromFile12(renderer->m_pDevice.Get(),
        renderer->m_pCommandList.Get(),
        L"res/texture/texture.dds",
        m_textureBuffer,
        m_uploadTexture,
        0,
        nullptr
    );
    ASSERT_FAILED(hr);


    // Describe and create a SRV for the texture.
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = m_textureBuffer->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = m_textureBuffer->GetDesc().MipLevels;
    renderer->m_pDevice->CreateShaderResourceView(m_textureBuffer.Get(), &srvDesc, renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());


    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    PRINT("Texture not Loaded");

    renderer->WaitForPreviousFrame();

    //hr = renderer->m_pCommandList->Close();
    


}


