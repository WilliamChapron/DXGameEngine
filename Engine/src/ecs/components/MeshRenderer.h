#pragma once


#include "../components/Component.h"
//#include "../components/Mesh.h"

class Mesh;

struct ConstantBufferData {
    XMFLOAT4X4 model;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};


class Renderer;

class MeshRenderer : public Component
{
public:
    MeshRenderer(std::string name, ConstantBufferData* m_cbData, Mesh* mesh);
    void UpdateConstantBuffer(ConstantBufferData* cbData);
    void Update(Renderer* renderer) override;

    void Initialize(Renderer* renderer, ConstantBufferData* cbData);

    inline ConstantBufferData* GetConstantBufferData() const { return m_cbData; }

    ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
    UINT8* GetMappedConstantBuffer() const { return m_mappedConstantBuffer; }

private:
    // Constant buffer
    ID3D12Resource* m_constantBuffer;
    UINT8* m_mappedConstantBuffer;
    //

    ConstantBufferData* m_cbData;
    Mesh* m_pMesh;
};
