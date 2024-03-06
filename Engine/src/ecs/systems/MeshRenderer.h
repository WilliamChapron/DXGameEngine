#pragma once


#include "../components/Component.h"
#include "../components/Mesh.h"

class Renderer;

class MeshRenderer : public Component
{
public:
    MeshRenderer(std::string name, ConstantBufferData* m_cbData, Mesh* mesh);
    void UpdateConstantBuffer(ConstantBufferData* cbData);
    void Update(Renderer* renderer) override;

    inline ConstantBufferData* GetConstantBufferData() const { return m_cbData; }

private:
    ConstantBufferData* m_cbData;
    Mesh* m_pMesh;
};
