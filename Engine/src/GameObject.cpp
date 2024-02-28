#include "GameObject.h"
#include "./renderer/Graphics.h"
#include "./core/Defines.h"

GameObject::GameObject() : m_transform(), m_vertexBuffer(nullptr)
{
    
}

GameObject::GameObject(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) : m_transform(position, rotation, scale)
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


    static float translationOffset = 0.0f;
    translationOffset -= 0.0003f;

    static float rotationAngle = 0.0f;
    rotationAngle += 0.01;

    static float fScale = 0.5f;
    fScale += 0.0001f;


    m_transformData.Translate(translationOffset, translationOffset, 0);
    m_cbData.model = m_transformData.GetTransformMatrix();
    /* Rotate*/



    m_transformData.Rotate(rotationAngle, rotationAngle, rotationAngle);



    m_transformData.Scale(fScale, fScale, fScale);
    m_cbData.model = m_transformData.GetTransformMatrix();



    // Map 
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);



}

void GameObject::UpdateDrawingOperations(Renderer* renderer)
{
    HRESULT hr;

    // * Update constant buffer 
    D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_constantBuffer->GetGPUVirtualAddress();
    renderer->m_pCommandList->SetGraphicsRootConstantBufferView(0, cbvAddress);
    // Update constant buffer * 

    // Record commands.
    const float clearColor[] = { 0.3f, 0.8f, 0.1f, 1.0f };
    renderer->m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_);
    renderer->m_pCommandList->IASetIndexBuffer(&m_indexBufferView);

    renderer->m_pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}
