#include "Mesh.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "Component.h"
#include "../entities/GameObject.h"

MeshComponent::MeshComponent(std::string name, ConstantBufferData* _m_cbData) : Component(name, ComponentType::Mesh, false), m_cbData(_m_cbData)
{
}

void MeshComponent::Initialize(Renderer* renderer)
{
	// Définition des données des sommets du cube
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

	// Définition des indices du cube
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

	// Calcul des tailles des tampons de sommets et d'indices
	const UINT vertexBufferSize = sizeof(cubeVertices);// *sizeof(Vertex);
	const UINT indexBufferSize = sizeof(cubeIndices);// *sizeof(UINT);
	const UINT stride = sizeof(Vertex);

	// Création des tampons de sommets et d'indices
	CreateIndexBuffer(indexBufferSize, cubeIndices, m_indexBuffer, m_indexBufferView, renderer);
	CreateVertexBuffer(vertexBufferSize, cubeVertices, m_vertexBuffer, m_vertexBufferView, stride, renderer);

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
	//ConstantBufferData tmp = *m_cbData;

	// Mappage du tampon de constantes pour la mise à jour des données
	hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
	ASSERT_FAILED(hr);
	//m_cbData->model

	// Copie des données des constantes mises à jour
	memcpy(m_mappedConstantBuffer, m_cbData, sizeof(ConstantBufferData));
	m_constantBuffer->Unmap(0, nullptr);

	// Configuration du descripteur de table de racine pour le tampon de constantes
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvHandle(renderer->m_pCbvSrvHeap.Get()->GetGPUDescriptorHandleForHeapStart());
	renderer->m_pCommandList->SetGraphicsRootDescriptorTable(0, cbvSrvHandle);

	// Mise à jour du tampon de constantes sur le périphérique graphique
	D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_constantBuffer->GetGPUVirtualAddress();
	renderer->m_pCommandList->SetGraphicsRootConstantBufferView(1, cbvAddress);

	// Configuration des données de rendu
	renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	renderer->m_pCommandList->IASetIndexBuffer(&m_indexBufferView);

	// Dessin du maillage
	renderer->m_pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);

    PRINT("Update Mesh OK");
}