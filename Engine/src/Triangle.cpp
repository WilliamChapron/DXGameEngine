#include "Triangle.h"
#include "renderer/Renderer.h"
#include "core/Defines.h"
#include <stdexcept>

HRESULT CompileShaderFromFile(const wchar_t* filePath, const char* entryPoint, const char* shaderModel, ID3DBlob** blob)
{
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, blob, &errorBlob);

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(static_cast<const char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
        return hr;
    }

    if (errorBlob)
        errorBlob->Release();


    PRINT("OK");

    return S_OK;
}



Triangle::Triangle() : vertexBuffer(nullptr), vertexShaderBlob(nullptr), pixelShaderBlob(nullptr) {
    
}

Triangle::~Triangle() {
    
}

void Triangle::Initialize(Renderer* renderer) {
    // Vertices du triangle
    Vertex triangleVertices[] = {
        {XMFLOAT3(0.0f, 0.5f, 0.0f)},
        {XMFLOAT3(0.5f, -0.5f, 0.0f)},
        {XMFLOAT3(-0.5f, -0.5f, 0.0f)}
    };



    // Charger les shaders spécifiques au Triangle
    CompileShaderFromFile(L"res/shader/VS.hlsl", "main", "vs_5_0", &vertexShaderBlob);
    CompileShaderFromFile(L"res/shader/PS.hlsl", "main", "ps_5_0", &pixelShaderBlob);

    // Création du vertex buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(triangleVertices));

    renderer->pDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer)
    );

    UINT8* pVertexDataBegin;

    CD3DX12_RANGE readRange(0, 0);

    vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));

    memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));

    vertexBuffer->Unmap(0, nullptr);

    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = sizeof(Vertex);
    vertexBufferView.SizeInBytes = sizeof(triangleVertices);
}

void Triangle::WaitForPreviousFrame(Renderer* renderer)
{
    // Signal and increment the fence value.
    const UINT64 fence = renderer->fenceValue;
    renderer->pCommandQueue->Signal(renderer->pFence, fence);
    renderer->fenceValue++;

    // Wait until the previous frame is finished.
    if (renderer->pFence->GetCompletedValue() < fence)
    {
        renderer->pFence->SetEventOnCompletion(fence, renderer->pFence);
        WaitForSingleObject(renderer->pFence, INFINITE);
    }

    renderer->frameIndex = (renderer->frameIndex + 1) % renderer->FRAME_COUNT;
}

void Triangle::PopulateCommandList(Renderer* renderer)
{
    renderer->pCommandList->SetGraphicsRootSignature(renderer->pRootSignature);
    renderer->pCommandList->RSSetViewports(1, &m_viewport);
    renderer->pCommandList->RSSetScissorRects(1, &m_scissorRect);

    CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        renderer->pRenderTargets[renderer->frameIndex],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    renderer->pCommandList->ResourceBarrier(1, &transitionBarrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(renderer->pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), renderer->frameIndex, renderer->pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    renderer->pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    renderer->pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    renderer->pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->pCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);

    // Ajoutez la commande DrawInstanced pour dessiner le triangle
    renderer->pCommandList->DrawInstanced(3, 1, 0, 0);

    CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        renderer->pRenderTargets[renderer->frameIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    renderer->pCommandList->ResourceBarrier(1, &presentBarrier);

    renderer->pCommandList->Close();
}

void Triangle::Draw(Renderer* renderer)
{

    PopulateCommandList(renderer);

    ID3D12CommandList* ppCommandLists[] = { renderer->pCommandList };
    renderer->pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Présentez la frame.
    renderer->pSwapChain->Present(1, 0);

    WaitForPreviousFrame(renderer);
}

