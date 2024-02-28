#include "Triangle.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include <stdexcept>

#include "../../Utils.h"
#include <DirectXColors.h>

#include "../components/Camera.h"

#include "../../renderer/Resources.h"



Triangle::Triangle() {

}

Triangle::~Triangle() {

    if (m_vertexBuffer != nullptr) {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }
}


void Triangle::Initialize(Renderer* renderer, Camera* camera) {
    m_transformData = Transform();
    m_cbData.model = m_transformData.GetTransformMatrix();

    // ** MATRIX

    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();

    // MATRIX **

    Vertex cubeVertices[] = {
        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },  // Rouge
        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },  // Rouge
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },  // Bleu
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },  // Bleu
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },  // Jaune
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },  // Jaune
        { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f} },  // Gris
        { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f} }   // Gris
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

    HRESULT hr;

    CreateIndexBuffer(indexBufferSize, cubeIndices, m_indexBuffer, m_indexBufferView, renderer);
    CreateVertexBuffer(vertexBufferSize, cubeVertices, m_vertexBuffer, m_vertexBufferView, stride, renderer);



    //// Index Buffer
    //CD3DX12_HEAP_PROPERTIES heapPropsIndex(D3D12_HEAP_TYPE_UPLOAD);
    //CD3DX12_RESOURCE_DESC bufferDescIndex = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

    //hr = renderer->m_pDevice->CreateCommittedResource(
    //    &heapPropsIndex,
    //    D3D12_HEAP_FLAG_NONE,
    //    &bufferDescIndex,
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&m_indexBuffer)
    //);
    //ASSERT_FAILED(hr);

    //UINT8* pIndexDataBegin;
    //CD3DX12_RANGE readRangeIndex(0, 0);

    //hr = m_indexBuffer->Map(0, &readRangeIndex, reinterpret_cast<void**>(&pIndexDataBegin));
    //ASSERT_FAILED(hr);
    //memcpy(pIndexDataBegin, cubeIndices, indexBufferSize);
    //m_indexBuffer->Unmap(0, nullptr);

    //m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
    //m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    //m_indexBufferView.SizeInBytes = indexBufferSize;


    //

    //// Vertex Buffer * 
    //CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    //CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    //hr = renderer->m_pDevice->CreateCommittedResource(
    //    &heapProps,
    //    D3D12_HEAP_FLAG_NONE,
    //    &bufferDesc,
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&m_vertexBuffer)
    //);
    //ASSERT_FAILED(hr);

    //UINT8* pVertexDataBegin;
    //CD3DX12_RANGE readRange(0, 0);


    //hr = m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    //ASSERT_FAILED(hr);
    //memcpy(pVertexDataBegin, cubeVertices, vertexBufferSize);
    //m_vertexBuffer->Unmap(0, nullptr);

    //m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    //m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    //m_vertexBufferView.SizeInBytes = vertexBufferSize;



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
    renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_pCbvHeap->GetCPUDescriptorHandleForHeapStart());
    // Constant Buffer *

    PRINT("Triangle initialization complete");
}


