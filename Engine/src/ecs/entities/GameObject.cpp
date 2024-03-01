#include "GameObject.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"

GameObject::GameObject() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_constantBuffer(nullptr), m_mappedConstantBuffer(nullptr), m_vertexBufferView({}), m_indexBufferView({}), m_cbData()
{
}


void GameObject::Update(float deltaTime, Renderer* renderer)
{
    UpdateTransformation(deltaTime);
    UpdateDrawingOperations(renderer);
}

void GameObject::UpdateTransformation(float deltaTime)
{
    HRESULT hr;

    //PRINT("Update TRANSFORM");
    static float translationOffset = 0.0f;
    //translationOffset -= 0.0003f;

    static float rotationAngle = 0.03f;
    /*rotationAngle += 0.01;*/

    static float fScale = 0.1f;
    fScale += 0.01f;


    //m_transform.Translate(translationOffset, 0, 0);
    m_transform.Rotate(0, 0, rotationAngle);
    //m_transform.Scale(1, fScale, 1);


    m_cbData.model = m_transform.GetTransformMatrix();



    // Map 
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);



}

void GameObject::UpdateDrawingOperations(Renderer* renderer)
{
    HRESULT hr;

    // Update constant buffer SRV / Sampler
    
    // Link descriptors heap to command list || EACH FRAME ?
    ID3D12DescriptorHeap* heaps[] = { renderer->m_pCbvSrvHeap.Get() };
    renderer->m_pCommandList->SetDescriptorHeaps(_countof(heaps), heaps);

    // Link descriptors attach to shader || EACH FRAME ?

    CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvHandle(renderer->m_pCbvSrvHeap.Get()->GetGPUDescriptorHandleForHeapStart());
    renderer->m_pCommandList->SetGraphicsRootDescriptorTable(0, cbvSrvHandle);


    // * Update constant buffer 
    D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_constantBuffer->GetGPUVirtualAddress();
    renderer->m_pCommandList->SetGraphicsRootConstantBufferView(1, cbvAddress);
    // Update constant buffer * 
    //PRINT("Drawing Op");

    // Record commands.
    renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    renderer->m_pCommandList->IASetIndexBuffer(&m_indexBufferView);

    renderer->m_pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}
