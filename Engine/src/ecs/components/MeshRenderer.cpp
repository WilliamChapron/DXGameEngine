#include "../components/Mesh.h"
#include "MeshRenderer.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../components/Component.h"

#include "../../Utils.h"

MeshRenderer::MeshRenderer(std::string name, ConstantBufferData* cbData, Mesh* mesh) : Component(name, ComponentType::MeshRenderer)
{
    m_cbData = cbData;
    m_pMesh = mesh;
}

void MeshRenderer::Initialize(Renderer* renderer, ConstantBufferData* cbData) {
    //PRINT("INITIALIZE CONSTANT BUFFER");
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

    //std::cout << "Model Matrix during initialization:" << std::endl;
    //PrintMatrix(cbData->model);


    // #TODO FAIRE UNE FOIS PAR OBJET

    // Mappage du tampon de constantes pour la copie des données
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    // Copie des données des constantes
    memcpy(m_mappedConstantBuffer, cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);
}

void MeshRenderer::UpdateConstantBuffer(ConstantBufferData* cbData)
{
    m_cbData = cbData;
    //PRINT("Update Constant buffer");
    // Affichage de la matrice modèle
    //std::cout << "Model Matrix of" << GetName() << std::endl;
    //PrintMatrix(m_cbData->model);
}


void MeshRenderer::Update(Renderer* renderer) {
    
    //PRINT("Update Mesh renderer");
    HRESULT hr;

    //std::cout << "Model Matrix of" << GetName() << std::endl;
    //PrintMatrix(m_cbData->model);

    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(m_mappedConstantBuffer));
    ASSERT_FAILED(hr);

    memcpy(m_mappedConstantBuffer, m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);




    D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_constantBuffer->GetGPUVirtualAddress();
    renderer->m_pCommandList->SetGraphicsRootConstantBufferView(1, cbvAddress);

    renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_pMesh->GetVertexBufferView());
    renderer->m_pCommandList->IASetIndexBuffer(&m_pMesh->GetIndexBufferView());

    renderer->m_pCommandList->DrawIndexedInstanced(m_pMesh->GetNumIndices(), 1, 0, 0, 0);

    //PRINT("Update Mesh OK");
}