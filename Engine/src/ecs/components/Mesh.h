#pragma once

#include "../../include.h"
#include "../components/Component.h"
#include "../entities/GameObject.h"

class Renderer;

// Vertex
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT2 Uv;
};

class MeshComponent : public Component
{
public:
	MeshComponent(std::string name, ConstantBufferData* m_cbData);
	void Initialize(Renderer* renderer, Vertex* vertices, int numVertices, UINT* indices, int numIndices);
	void Update(Renderer* renderer) override;

private:
	ConstantBufferData* m_cbData;

	// Vertex buffer
	ID3D12Resource* m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Index buffer
	ID3D12Resource* m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	// Constant buffer
	ID3D12Resource* m_constantBuffer;
	UINT8* m_mappedConstantBuffer;

	int m_numVertices;
	int m_numIndices;

public:
	ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
	ID3D12Resource* GetIndexBuffer() const { return m_indexBuffer; }
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return m_vertexBufferView; }
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_indexBufferView; }

};
