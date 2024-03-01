#include "GameObject.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../components/Camera.h"

GameObject::GameObject() : m_transform(), m_vertexBuffer(nullptr), m_indexBuffer(nullptr)
{

}

GameObject::GameObject(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) : m_transform(position, rotation, scale)
{

}

void GameObject::Update(float deltaTime, Renderer* renderer, Camera* camera)
{
    UpdateTransformation(deltaTime);
    UpdateDrawingOperations(renderer, camera);
}

void GameObject::UpdateTransformation(float deltaTime)
{
    HRESULT hr;

    PRINT("Update TRANSFORM");
    static float translationOffset = 0.0f;
    //translationOffset -= 0.0003f;

    static float rotationAngle = 0.001f;
    /*rotationAngle += 0.01;*/

    static float fScale = 0.1f;
    fScale += 0.01f;


    //m_transformData.Translate(translationOffset, translationOffset, 0);
    //m_transformData.Rotate(0, 0, rotationAngle);
    //m_transformData.Scale(1, fScale, 1);


    m_cbData.model = m_transformData.GetTransformMatrix();



    // Map 
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);



}

void GameObject::UpdateDrawingOperations(Renderer* renderer, Camera* camera)
{
    HRESULT hr;
    
    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();

    // * Update constant buffer 
    D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_constantBuffer->GetGPUVirtualAddress();
    renderer->m_pCommandList->SetGraphicsRootConstantBufferView(0, cbvAddress);
    // Update constant buffer * 
    PRINT("Drawing Op");

    // Record commands.
    renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->m_pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    renderer->m_pCommandList->IASetIndexBuffer(&m_indexBufferView);

    renderer->m_pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}
