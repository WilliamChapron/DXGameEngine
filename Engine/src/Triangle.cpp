#include "Triangle.h"
#include "renderer/Graphics.h"
#include "core/Defines.h"
#include <stdexcept>

#include "Utils.h"
#include <DirectXColors.h> 



Triangle::Triangle() {

}

Triangle::~Triangle() {

    if (m_vertexBuffer != nullptr) {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }

    //PRINT("Triangle destruction complete");
}


//Vertex triangleVertices[] = {
    //    { { -0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    //    { { -0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    //    { { -0.5f,  0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    //    { { -0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
    //    { {  0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
    //    { {  0.5f, -0.5f,  0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    //    { {  0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    //    { {  0.5f,  0.5f,  0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
    //};// Imprimer les valeurs après le mappage


// Vertices du triangle
    //Vertex triangleVertices[] = {
    //    { { -0.2f, -0.2f, -0.2f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    //    { { -0.2f, 0.2f, 0.2f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    //    { { 0.2f, 0.2f, 0.2f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    //};



void Triangle::Initialize(Renderer* renderer) {

    

    m_transformData = Transform();
    m_cbData.model = m_transformData.GetTransformMatrix();

    PrintMatrix(m_cbData.model);


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
    // # TO THINK - Opacity?

    //Vertex cubeVertices[] = {
    //{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 0.0f} },  // Rouge
    //{ {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 0.0f} },  // Rouge
    //{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 0.0f} },  // Bleu
    //{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 0.0f} },  // Bleu
    //{ { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 0.0f} },  // Jaune
    //{ { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 0.0f} },  // Jaune
    //{ { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 0.0f} },  // Gris
    //{ { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 0.0f} }   // Gris
    //};


    //Vertex cubeVertices[] = {
    //    { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { {-0.5f,  0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { {-0.5f,  0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f,  0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} }
    //};


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


    
    const UINT vertexBufferSize = sizeof(cubeVertices);
    const UINT indexBufferSize = sizeof(cubeIndices);



    // Index Buffer
    CD3DX12_HEAP_PROPERTIES heapPropsIndex(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDescIndex = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

    HRESULT hr = renderer->m_pDevice->CreateCommittedResource(
        &heapPropsIndex,
        D3D12_HEAP_FLAG_NONE,
        &bufferDescIndex,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_indexBuffer)
    );
    ASSERT_FAILED(hr);

    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readRangeIndex(0, 0);

    hr = m_indexBuffer->Map(0, &readRangeIndex, reinterpret_cast<void**>(&pIndexDataBegin));
    ASSERT_FAILED(hr);
    memcpy(pIndexDataBegin, cubeIndices, indexBufferSize);
    m_indexBuffer->Unmap(0, nullptr);

    m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
    m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    m_indexBufferView.SizeInBytes = indexBufferSize;



    // ** MATRIX

    // Initialisation de la matrice de vue (view)
    XMFLOAT3 eye(0.0f, 0.0f, -2.0f);    // Position de la caméra
    XMFLOAT3 at(0.0f, 0.0f, 0.0f);      // Point où la caméra regarde
    XMFLOAT3 up(0.0f, 1.0f, 0.0f);      // Vecteur "up" (orienté vers le haut)
    XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up));
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
    XMStoreFloat4x4(&m_cbData.view, transposedViewMatrix);

    // Initialisation de la matrice de projection
    float aspectRatio = 16.0f / 9.0f;   // Vous devrez fournir la valeur de l'aspect ratio
    float fieldOfView = XM_PIDIV4;  // Angle de champ de vision (45 degrés ici)
    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, 0.1f, 100.0f);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMStoreFloat4x4(&m_cbData.projection, transposedProjectionMatrix);

    // MATRIX **




    // Vertex Buffer * 
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    hr = renderer->m_pDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)
    );
    ASSERT_FAILED(hr);

    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);



    hr = m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    ASSERT_FAILED(hr);
    memcpy(pVertexDataBegin, cubeVertices, vertexBufferSize);
    m_vertexBuffer->Unmap(0, nullptr);

    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;



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

    //static float translationAngle = 0.0f;
    //translationAngle += 10.0f;
    //m_transformData.Translate(translationAngle, translationAngle, 0);
    //m_cbData.model = m_transformData.GetTransformMatrix();

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


