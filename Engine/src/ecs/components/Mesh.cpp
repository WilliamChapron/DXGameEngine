#include "Mesh.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "Component.h"
#include "../entities/GameObject.h"


MeshComponent::MeshComponent(std::string name) : Component(name, ComponentType::Mesh, false)
{

}

void MeshComponent::Initialize(Renderer* renderer, ConstantBufferData* _m_cbData)
{
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

    HRESULT hr;

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

    memcpy(m_mappedConstantBuffer, &_m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);
}




void MeshComponent::Update(Renderer* renderer) {
    PRINT("Update Mesh");
}