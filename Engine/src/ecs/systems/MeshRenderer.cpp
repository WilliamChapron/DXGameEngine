#include "../components/Mesh.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../components/Component.h"
#include "MeshRenderer.h"
#include "../../Utils.h"

MeshRenderer::MeshRenderer(std::string name, ConstantBufferData* cbData, Mesh* mesh) : Component(name, ComponentType::MeshRenderer, false)
{
    m_cbData = cbData;
    m_pMesh = mesh;
}

void MeshRenderer::UpdateConstantBuffer(ConstantBufferData* cbData)
{
    m_cbData = cbData;
}


void MeshRenderer::Update(Renderer* renderer) {

    HRESULT hr;
    hr = m_pMesh->GetConstantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(m_pMesh->GetMappedConstantBuffer()));
    ASSERT_FAILED(hr);

    memcpy(m_pMesh->GetMappedConstantBuffer(), m_cbData, sizeof(ConstantBufferData));
    m_pMesh->GetConstantBuffer()->Unmap(0, nullptr);


    // Affichage de la matrice modèle
    //std::cout << "Model Matrix of" << GetName() << std::endl;
    //PrintMatrix(m_cbData->model);

    D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_pMesh->GetConstantBuffer()->GetGPUVirtualAddress();
    renderer->m_pCommandList->SetGraphicsRootConstantBufferView(1, cbvAddress);

    renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_pMesh->GetVertexBufferView());
    renderer->m_pCommandList->IASetIndexBuffer(&m_pMesh->GetIndexBufferView());

    renderer->m_pCommandList->DrawIndexedInstanced(m_pMesh->GetNumIndices(), 1, 0, 0, 0);

    PRINT("Update Mesh OK");
}