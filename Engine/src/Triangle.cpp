#include "Triangle.h"
#include "renderer/Renderer.h"
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
        { { 0.0f, 0.45f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };


    PRINT("Initializing Triangle...");

    // Création du vertex buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(triangleVertices));

    HRESULT hr = renderer->pDevice->CreateCommittedResource(
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

    PRINT("Triangle initialization complete");
}

void Triangle::WaitForPreviousFrame(Renderer* renderer)
{
    PRINT("Waiting for previous frame...");

    // Signal and increment the fence value.


    renderer->fenceValue++;

    HRESULT hr = renderer->pCommandQueue->Signal(renderer->pFence.Get(), renderer->fenceValue);
    ASSERT_FAILED(hr);

    // Wait until the previous frame is finished.
    if (renderer->pFence->GetCompletedValue() < renderer->fenceValue)
    {
        //hr = renderer->pFence->SetEventOnCompletion(renderer->fenceValue, renderer->fenceEvent);
        //ASSERT_FAILED(hr);

        //WaitForSingleObject(renderer->pFence.Get(), INFINITE); //#ASK Attendre Event Handle ou fence? marche pas #ASK Porblème fence Synchro

        //Check if the GPU has completed all commands associated with the previous fence value
        HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);

        //Set the event to the current fence value
        hr = renderer->pFence->SetEventOnCompletion(renderer->fenceValue, eventHandle);
        ASSERT_FAILED(hr);

        //Wait for the GPU to complete associated commands
        WaitForSingleObject(eventHandle, INFINITE);

        //Close the handle to the event
        CloseHandle(eventHandle);
    }


    PRINT("frameIndex");
    PRINT(renderer->frameIndex);
    //renderer->frameIndex = renderer->pSwapChain->GetCurrentBackBufferIndex();

    PRINT("Previous frame completed");
}



void Triangle::PopulateCommandList(Renderer* renderer)
{
    HRESULT hr;
    PRINT("Populating command list...");

    hr = renderer->pCommandAllocator->Reset();
    ASSERT_FAILED(hr);

    hr = renderer->pCommandList->Reset(renderer->pCommandAllocator.Get(), renderer->pPipelineState.Get());
    ASSERT_FAILED(hr);

    renderer->pCommandList->SetGraphicsRootSignature(renderer->pRootSignature.Get());
    renderer->pCommandList->RSSetViewports(1, renderer->pViewport);
    renderer->pCommandList->RSSetScissorRects(1, renderer->pScissorRect);

    CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        renderer->pRenderTargets[renderer->frameIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    renderer->pCommandList->ResourceBarrier(1, &transitionBarrier);


    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        renderer->pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        renderer->frameIndex,
        renderer->rtvDescriptorSize);

    renderer->pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.3f, 0.8f, 0.1f, 1.0f };
    renderer->pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    renderer->pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->pCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);

    // Ajoutez la commande DrawInstanced pour dessiner le triangle
    PRINT("Draw Success-->");
    renderer->pCommandList->DrawInstanced(3, 1, 0, 0);
    PRINT("Draw Success<--");

    CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        renderer->pRenderTargets[renderer->frameIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    renderer->pCommandList->ResourceBarrier(1, &presentBarrier);

    hr = renderer->pCommandList->Close();  // Assurez-vous que Close est appelé même en cas d'erreur
    ASSERT_FAILED(hr);

    PRINT("Command list populated");
}


void Triangle::Render(Renderer* renderer)
{
    HRESULT hr;

    PRINT("Rendering...");

    PopulateCommandList(renderer);

    ID3D12CommandList* ppCommandLists[] = { renderer->pCommandList.Get()};
    renderer->pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Présentez la frame.
    hr = renderer->pSwapChain->Present(1, 0);
    renderer->frameIndex = (renderer->frameIndex + 1) % renderer->FRAME_COUNT;
    ASSERT_FAILED(hr);


    WaitForPreviousFrame(renderer);

    PRINT("Rendering complete");

    renderCallNum++;
    PRINT(renderCallNum++);

}
