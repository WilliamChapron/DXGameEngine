#pragma once

#include "../../include.h"
#include "../components/Component.h"
#include "../entities/GameObject.h"

class Renderer;

class MeshComponent : public Component
{
public:
	MeshComponent(std::string name);
	void Initialize(Renderer* renderer, ConstantBufferData* _m_cbData);
	void Update(Renderer* renderer);

private:
    ID3D12Resource* m_constantBuffer;
    UINT8* m_mappedConstantBuffer;

    // Vertex buffer
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    ID3D12Resource* m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};
