#pragma once

#include "./Resources.h"
#include "../core/Defines.h"
#include "./Graphics.h"
#include "../ecs/entities/GameObject.h"

void CreateIndexBuffer(const UINT& size, UINT* indices, ID3D12Resource* buffer, D3D12_INDEX_BUFFER_VIEW& bufferView, Renderer* renderer) {
    HRESULT hr;
    // Index Buffer
    CD3DX12_HEAP_PROPERTIES heapPropsIndex(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDescIndex = CD3DX12_RESOURCE_DESC::Buffer(size);

    hr = renderer->m_pDevice->CreateCommittedResource(
        &heapPropsIndex,
        D3D12_HEAP_FLAG_NONE,
        &bufferDescIndex,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer)
    );
    ASSERT_FAILED(hr);

    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readRangeIndex(0, 0);

    hr = buffer->Map(0, &readRangeIndex, reinterpret_cast<void**>(&pIndexDataBegin));
    ASSERT_FAILED(hr);
    memcpy(pIndexDataBegin, indices, size);
    buffer->Unmap(0, nullptr);

    bufferView.BufferLocation = buffer->GetGPUVirtualAddress();
    bufferView.Format = DXGI_FORMAT_R32_UINT;
    bufferView.SizeInBytes = size;


}

void CreateVertexBuffer(const UINT& size, Vertex* vertices, ID3D12Resource* buffer, D3D12_VERTEX_BUFFER_VIEW& bufferView, const UINT& stride, Renderer* renderer) {
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

    HRESULT hr = renderer->m_pDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer)
    );
    ASSERT_FAILED(hr);

    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);

    hr = buffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    ASSERT_FAILED(hr);
    memcpy(pVertexDataBegin, vertices, size);

    buffer->Unmap(0, nullptr);

    bufferView.BufferLocation = buffer->GetGPUVirtualAddress();
    bufferView.StrideInBytes = stride;
    bufferView.SizeInBytes = size;
}
