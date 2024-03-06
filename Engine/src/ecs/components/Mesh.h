#pragma once

#include "../../include.h"
#include "../components/Component.h"
#include "../entities/GameObject.hpp"


class Renderer;

struct ConstantBufferData {
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

// Vertex
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT2 Uv;
};

class Mesh
{
public:
	Mesh();
	void Initialize(ConstantBufferData* cbData, Renderer* renderer, Vertex* vertices, int numVertices, UINT* indices, int numIndices);

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
	ConstantBufferData* GetConstantBufferData() const { return m_cbData; }
	ID3D12Resource* GetVertexBuffer() const { return m_vertexBuffer; }
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return m_vertexBufferView; }
	ID3D12Resource* GetIndexBuffer() const { return m_indexBuffer; }
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_indexBufferView; }
	ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
	UINT8* GetMappedConstantBuffer() const { return m_mappedConstantBuffer; }
	int GetNumVertices() const { return m_numVertices; }
	int GetNumIndices() const { return m_numIndices; }

};
