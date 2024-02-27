#include "Triangle.h"
#include "renderer/Graphics.h"
#include "core/Defines.h"
#include <stdexcept>

#include <DirectXColors.h> 







Triangle::Triangle() : vertexBuffer(nullptr){

}

Triangle::~Triangle() {

    if (vertexBuffer != nullptr) {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }

    PRINT("Triangle destruction complete");
}



void Triangle::Initialize(Renderer* renderer) {

    // Vertices du triangle
    Vertex triangleVertices[] = {
        { { 0.0f, 0.45f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    //Vertex triangleVertices[] = {
    //    { { -0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    //    { { -0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    //    { { -0.5f,  0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    //    { { -0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
    //    { {  0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
    //    { {  0.5f, -0.5f,  0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    //    { {  0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    //    { {  0.5f,  0.5f,  0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
    //};

    PRINT("Initializing Triangle...");

    // Création du vertex buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(triangleVertices));

    HRESULT hr = renderer->m_pDevice->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer)
    );
    ASSERT_FAILED(hr);

    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);



    hr = vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    ASSERT_FAILED(hr);

    memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));

    vertexBuffer->Unmap(0, nullptr);

    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = sizeof(Vertex);
    vertexBufferView.SizeInBytes = sizeof(triangleVertices);


    // Init Constant buffer 

    CD3DX12_HEAP_PROPERTIES cbHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC cbDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(m_worldViewProj) + 255) & ~255);
    hr = renderer->m_pDevice->CreateCommittedResource(
        &cbHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &cbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)
    );
    ASSERT_FAILED(hr);

    // Créer la vue de tampon de constantes
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = (sizeof(m_worldViewProj) + 255) & ~255; // Alignement sur 256 octets
    renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_pCbvHeap->GetCPUDescriptorHandleForHeapStart());

    // Mappage initial du tampon de constantes
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);

    // Initialisation des données du tampon de constantes
    ZeroMemory(m_constantBuffer, sizeof(m_constantBuffer));

    PRINT("Triangle initialization complete");
}

void Triangle::WaitForPreviousFrame(Renderer* renderer)
{
    PRINT("Waiting for previous frame...");

    // Signal and increment the fence value.


    renderer->m_fenceValue++;

    HRESULT hr = renderer->m_pCommandQueue->Signal(renderer->m_pFence.Get(), renderer->m_fenceValue);
    ASSERT_FAILED(hr);

    // Wait until the previous frame is finished.
    if (renderer->m_pFence->GetCompletedValue() < renderer->m_fenceValue)
    {
        //Check if the GPU has completed all commands associated with the previous fence value
        HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);

        //Set the event to the current fence value
        hr = renderer->m_pFence->SetEventOnCompletion(renderer->m_fenceValue, eventHandle);
        ASSERT_FAILED(hr);

        //Wait for the GPU to complete associated commands
        WaitForSingleObject(eventHandle, INFINITE);

        //Close the handle to the event
        CloseHandle(eventHandle);
    }


    PRINT("frameIndex");
    PRINT(renderer->m_frameIndex);
    //renderer->frameIndex = renderer->pSwapChain->GetCurrentBackBufferIndex();

    PRINT("Previous frame completed");
}



void Triangle::PopulateCommandList(Renderer* renderer)
{
    HRESULT hr;
    PRINT("Populating command list...");

    hr = renderer->m_pCommandAllocator->Reset();
    ASSERT_FAILED(hr);

    hr = renderer->m_pCommandList->Reset(renderer->m_pCommandAllocator.Get(), renderer->m_pPipelineState.Get());
    ASSERT_FAILED(hr);

    renderer->m_pCommandList->SetGraphicsRootSignature(renderer->m_pRootSignature.Get());
    renderer->m_pCommandList->RSSetViewports(1, renderer->m_pViewport);
    renderer->m_pCommandList->RSSetScissorRects(1, renderer->m_pScissorRect);

    CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        renderer->m_pRenderTargets[renderer->m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    renderer->m_pCommandList->ResourceBarrier(1, &transitionBarrier);


    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        renderer->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
        renderer->m_frameIndex,
        renderer->m_rtvDescriptorSize);


    // Update constant buffer * 


    //commandList->SetGraphicsRootConstantBufferView(0, );


    renderer->m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.3f, 0.8f, 0.1f, 1.0f };
    renderer->m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->m_pCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);

    // Ajoutez la commande DrawInstanced pour dessiner le triangle
    PRINT("Draw Success-->");
    renderer->m_pCommandList->DrawInstanced(3, 1, 0, 0);
    PRINT("Draw Success<--");

    CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        renderer->m_pRenderTargets[renderer->m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    renderer->m_pCommandList->ResourceBarrier(1, &presentBarrier);

    hr = renderer->m_pCommandList->Close();  // Assurez-vous que Close est appelé même en cas d'erreur
    ASSERT_FAILED(hr);

    PRINT("Command list populated");
}


void Triangle::Render(Renderer* renderer)
{
    HRESULT hr;

    PRINT("Rendering...");

    PopulateCommandList(renderer);

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get()};
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Présentez la frame.
    hr = renderer->m_pSwapChain->Present(1, 0);
    renderer->m_frameIndex = (renderer->m_frameIndex + 1) % renderer->m_FRAME_COUNT;
    ASSERT_FAILED(hr);


    WaitForPreviousFrame(renderer);

    PRINT("Rendering complete");

    renderCallNum++;
    PRINT(renderCallNum++);

}
