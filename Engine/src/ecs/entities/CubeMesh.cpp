

#include "CubeMesh.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include <stdexcept>

#include "../../Utils.h"
#include <DirectXColors.h>

#include "../components/Camera.h"
#include "../components/Texture.h"
#include "../components/Transform.h"

#include "../systems/ComponentManager.h"

#include "../../renderer/Resources.h"
#include "../../DDSTextureLoader.h"

#include "../components/Component.h"  





CubeMesh::CubeMesh() : GameObject()
{
}

CubeMesh::~CubeMesh() {

    if (m_vertexBuffer != nullptr) {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }
}


void CubeMesh::Initialize(Renderer* renderer, Camera* camera, ComponentManager* componentManager,  const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) {
    //m_transform = Transform(position, rotation, scale);
    //m_cbData.model = m_transform.GetTransformMatrix();

    

    Transform* defaultTransform = new Transform(position, rotation, scale);
    TextureComponent* defaultTexture = new TextureComponent("texture");
    defaultTexture->Initialize(renderer);

    componentManager->AddComponent(*this, defaultTransform);
    componentManager->AddComponent(*this, defaultTexture);
    
    //defaultTransform->Update();


    m_cbData.model = defaultTransform->GetTransformMatrix();

    // ** MATRIX

    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();

    // MATRIX **
    HRESULT hr;

    Vertex cubeVertices[] = {
        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  
        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },  
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },  
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  
        { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },  
        { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }   
    };

    UINT cubeIndices[] = {
        0, 1, 2,   
        2, 1, 3,   
        4, 6, 5,   
        6, 7, 5,   
        0, 2, 4,   
        2, 6, 4,   
        1, 5, 3,   
        3, 5, 7,   
        2, 3, 6,   
        3, 7, 6,   
        0, 4, 1,   
        1, 4, 5    
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


    //D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    //cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
    //cbvDesc.SizeInBytes = (sizeof(ConstantBufferData) + 255) & ~255; // Alignement sur 256 octets
   // renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
    // Constant Buffer *
}


