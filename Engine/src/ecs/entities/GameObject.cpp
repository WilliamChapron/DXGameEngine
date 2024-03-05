#include "GameObject.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../../Utils.h"

#include "../components/Component.h"  
#include "../systems/ComponentManager.h"  

#include "../../core/Engine.h"  
#include "../components/Camera.h"


#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include <stdexcept>

#include "../../Utils.h"
#include <DirectXColors.h>

#include "../components/Camera.h"
#include "../components/Texture.h"
#include "../components/Transform.h"
#include "../components/Mesh.h"

#include "../../renderer/Resources.h"



#include <iostream>
#include <iomanip>

GameObject::GameObject() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_constantBuffer(nullptr), m_mappedConstantBuffer(nullptr), m_vertexBufferView({}), m_indexBufferView({}), m_cbData()
{
}

void GameObject::Initialize(Renderer* renderer, Camera* camera, ComponentManager* componentManager, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) {
    //m_transform = Transform(position, rotation, scale);
    //m_cbData.model = m_transform.GetTransformMatrix();

    m_pComponentManager = componentManager;



    Transform* defaultTransform = new Transform(position, rotation, scale);
    TextureComponent* defaultTexture = new TextureComponent("texture");
    //MeshComponent* defaultMesh = new MeshComponent("Mesh");
    //defaultMesh->Initialize(renderer);
    defaultTexture->Initialize(renderer);


    componentManager->AddComponent(*this, defaultTransform);
    componentManager->AddComponent(*this, defaultTexture);
    //componentManager->AddComponent(*this, defaultMesh);

    //defaultTransform->Update();


    m_cbData.model = defaultTransform->GetTransformMatrix();

    // ** MATRIX

    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();

    // MATRIX **
    HRESULT hr;

    Vertex cubeVertices[] = {
        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
        { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },
        { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }
    };

    UINT cubeIndices[] = {
        0, 1, 2,
        2, 1, 3,
        4, 6, 5,
        6, 7, 5,
        0, 2, 4,
        2, 6, 4,
        1, 5, 3,
        3, 5, 7,
        2, 3, 6,
        3, 7, 6,
        0, 4, 1,
        1, 4, 5
    };

    const UINT vertexBufferSize = sizeof(cubeVertices);// *sizeof(Vertex);
    const UINT indexBufferSize = sizeof(cubeIndices);// *sizeof(UINT);
    const UINT stride = sizeof(Vertex);


    CreateIndexBuffer(indexBufferSize, cubeIndices, m_indexBuffer, m_indexBufferView, renderer);
    CreateVertexBuffer(vertexBufferSize, cubeVertices, m_vertexBuffer, m_vertexBufferView, stride, renderer);

    // * Constant Buffer
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

    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);

    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);


    //D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    //cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
    //cbvDesc.SizeInBytes = (sizeof(ConstantBufferData) + 255) & ~255; // Alignement sur 256 octets
   // renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
    // Constant Buffer *
}


template <typename T>
T* GameObject::GetComponent(ComponentType type) {
    Component* component = m_pComponentManager->GetComponentByType(*this, type);
    return dynamic_cast<T*>(component);
}



void GameObject::Update(float deltaTime, Renderer* renderer, Camera* camera)
{
    HRESULT hr;

    
    Transform* transformComponent = this->GetComponent<Transform>(ComponentType::Transform);

    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();



    float rotationAngle = 90.0;
    float rotationOffset = 0.01;
    //PRINT("Translation offset");
    //PRINT(m_transform.vPosition.z);
    float translationOffset = transformComponent->GetPosition().z + 0.01f;
    float fScale = transformComponent->GetScale().z - 0.001f;
    //printFloatWithPrecision(m_transform.vScale.z, 4);


    //m_transform.Translate(m_transform.vPosition.x, m_transform.vPosition.y, translationOffset);
    transformComponent->Rotate(0.003, 0, 0);
    //printFloatWithPrecision(transformComponent->GetRotation().z,1);
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

