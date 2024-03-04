#include "GameObject.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../../Utils.h"

#include "../components/Component.h"  
#include "../systems/ComponentManager.h"  

#include "../../core/Engine.h"  
#include "../components/Camera.h"


#include <iostream>
#include <iomanip>

GameObject::GameObject() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_constantBuffer(nullptr), m_mappedConstantBuffer(nullptr), m_vertexBufferView({}), m_indexBufferView({}), m_cbData()
{
}


void GameObject::Update(float deltaTime, Renderer* renderer, Camera* camera)
{
    HRESULT hr;

    Engine& e = Engine::GetInstance();
    Component* component = e.m_pComponentManager->GetComponentByType(*this, ComponentType::Transform);
    Transform* transformComponent = dynamic_cast<Transform*>(component);
    
    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();



    float rotationAngle = 0.03;
    float rotationOffset = 0.01;
    //PRINT("Translation offset");
    //PRINT(m_transform.vPosition.z);
    float translationOffset = transformComponent->GetPosition().z + 0.01f;
    float fScale = transformComponent->GetScale().z - 0.001f;
    //printFloatWithPrecision(m_transform.vScale.z, 4);


    //m_transform.Translate(m_transform.vPosition.x, m_transform.vPosition.y, translationOffset);
    transformComponent->Rotate(0, 0, rotationAngle);
    //m_transform.Scale(fScale, fScale, fScale);


    m_cbData.model = transformComponent->GetTransformMatrix();

    // Map 
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);


    // Update constant buffer SRV / Sampler

    // Link descriptors heap to command list || EACH FRAME ?

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


