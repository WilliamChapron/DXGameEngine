#include "GameObject.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"

#include <iostream>
#include <iomanip>

GameObject::GameObject() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_constantBuffer(nullptr), m_mappedConstantBuffer(nullptr), m_vertexBufferView({}), m_indexBufferView({}), m_cbData()
{
}


void GameObject::Update(float deltaTime, Renderer* renderer)
{
    UpdateTransformation(deltaTime);
    UpdateDrawingOperations(renderer);
}

void printFloatWithPrecision(float value, int precision) {
    std::cout << std::fixed << std::setprecision(precision) << value << std::endl;
}


void GameObject::UpdateTransformation(float deltaTime)
{
    HRESULT hr;

    ////PRINT("Update TRANSFORM");
    //static float translationOffset = 0.0f;
    ////translationOffset -= 0.0003f;

    //static float rotationAngle = 0.03f;
    ///*rotationAngle += 0.01;*/

    //static float fScale = 0.1f;
    //fScale += 0.01f;


    float rotationAngle = 0.03;
    float rotationOffset = 0.01;
    //PRINT("Translation offset");
    //PRINT(m_transform.vPosition.z);
    float translationOffset = m_transform.vPosition.z + 0.01f;
    float fScale = m_transform.vScale.z - 0.001f;
    //printFloatWithPrecision(m_transform.vScale.z, 4);

    
    //m_transform.Translate(m_transform.vPosition.x, m_transform.vPosition.y, translationOffset);
    m_transform.Rotate(0, 0, rotationAngle);
    //m_transform.Scale(fScale, fScale, fScale);


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
