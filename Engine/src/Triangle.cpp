#include "Triangle.h"
#include <stdexcept>

Triangle::Triangle() : vertexBuffer(nullptr) {
    // Initialiser vos membres ici si nécessaire
}

Triangle::~Triangle() {
    // Libérer les ressources ici si nécessaire
    if (vertexBuffer) {
        vertexBuffer->Release();
    }
}

void Triangle::Initialize(ID3D12Device* device) {
    // Vertices du triangle
    Vertex triangleVertices[] = {
        {XMFLOAT3(0.0f, 0.5f, 0.0f)},
        {XMFLOAT3(0.5f, -0.5f, 0.0f)},
        {XMFLOAT3(-0.5f, -0.5f, 0.0f)}
    };

    // Création du vertex buffer
    CreateVertexBuffer(device, triangleVertices, sizeof(triangleVertices));
}

void Triangle::Draw(ID3D12GraphicsCommandList* pCommandList) {
    // Configurer les descripteurs de ressources dans la commande liste (si nécessaire)
    // ...

    // Configurer le vertex buffer dans la commande liste
    pCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);

    // Configurer le pipeline state et la root signature
    // Utiliser device, pPipelineState, pRootSignature comme avant

    // Appeler DrawInstanced pour dessiner le triangle
    pCommandList->DrawInstanced(3, 1, 0, 0);
}

void Triangle::CreateVertexBuffer(ID3D12Device* device, const Vertex* vertices, UINT vertexSize) {
    // Créer un tampon de ressources pour le vertex buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexSize * 3);
    device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer)
    );

    // Remplir le vertex buffer avec les données
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0); // Pas besoin de lire avant CPU
    vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    memcpy(pVertexDataBegin, vertices, vertexSize * 3);
    vertexBuffer->Unmap(0, nullptr);

    // Configurer la vue du vertex buffer
    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = vertexSize;
    vertexBufferView.SizeInBytes = vertexSize * 3;
}
