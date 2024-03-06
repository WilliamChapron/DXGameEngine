#include "Mesh.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "Component.h"
#include "../entities/GameObject.h"

MeshComponent::MeshComponent(std::string name, ConstantBufferData* _m_cbData) : Component(name, ComponentType::Mesh, false), m_cbData(_m_cbData)
{
}

void MeshComponent::Initialize(Renderer* renderer, Vertex* vertices, int numVertices, UINT* indices, int numIndices)
{
    // Calcul des tailles des tampons de sommets et d'indices
    const UINT vertexBufferSize = sizeof(Vertex) * numVertices;
    const UINT indexBufferSize = sizeof(UINT) * numIndices;
    const UINT stride = sizeof(Vertex);

    m_numVertices = numVertices;
    m_numIndices = numIndices;

    //PRINT("NUMelement");
    //PRINT(numElementsI);

    // Création des tampons de sommets et d'indices
    CreateIndexBuffer(indexBufferSize, indices, m_indexBuffer, m_indexBufferView, renderer);
    CreateVertexBuffer(vertexBufferSize, vertices, m_vertexBuffer, m_vertexBufferView, stride, renderer);

    // Création du tampon de constantes
    HRESULT hr;
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

    // Mappage du tampon de constantes pour la copie des données
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    // Copie des données des constantes
    memcpy(m_mappedConstantBuffer, m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);
}

void MeshComponent::Update(Renderer* renderer) {

	HRESULT hr;
	hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
	ASSERT_FAILED(hr);

	memcpy(m_mappedConstantBuffer, m_cbData, sizeof(ConstantBufferData));
	m_constantBuffer->Unmap(0, nullptr);


	D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_constantBuffer->GetGPUVirtualAddress();
	renderer->m_pCommandList->SetGraphicsRootConstantBufferView(1, cbvAddress);

	renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	renderer->m_pCommandList->IASetIndexBuffer(&m_indexBufferView);

	renderer->m_pCommandList->DrawIndexedInstanced(m_numIndices, 1, 0, 0, 0);

    PRINT("Update Mesh OK");
}