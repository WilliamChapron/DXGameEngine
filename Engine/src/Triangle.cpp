#include "Triangle.h"
#include "renderer/Graphics.h"
#include "core/Defines.h"
#include <stdexcept>

#include "Utils.h"
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


//Vertex triangleVertices[] = {
    //    { { -0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    //    { { -0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    //    { { -0.5f,  0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    //    { { -0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
    //    { {  0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
    //    { {  0.5f, -0.5f,  0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    //    { {  0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    //    { {  0.5f,  0.5f,  0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
    //};// Imprimer les valeurs après le mappage


// Vertices du triangle
    //Vertex triangleVertices[] = {
    //    { { -0.2f, -0.2f, -0.2f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    //    { { -0.2f, 0.2f, 0.2f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    //    { { 0.2f, 0.2f, 0.2f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    //};



void Triangle::Initialize(Renderer* renderer) {

    

    m_transformData = Transform();
    m_cbData.model = m_transformData.GetTransformMatrix();

    PrintMatrix(m_cbData.model);


    Vertex cubeVertices[] = {
        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },  // Rouge
        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },  // Rouge
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },  // Bleu
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },  // Bleu
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },  // Jaune
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },  // Jaune
        { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f} },  // Gris
        { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f} }   // Gris
    };
    // # TO THINK - Opacity?

    //Vertex cubeVertices[] = {
    //{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 0.0f} },  // Rouge
    //{ {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 0.0f} },  // Rouge
    //{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 0.0f} },  // Bleu
    //{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 0.0f} },  // Bleu
    //{ { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 0.0f} },  // Jaune
    //{ { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 0.0f} },  // Jaune
    //{ { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 0.0f} },  // Gris
    //{ { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 0.0f} }   // Gris
    //};


    //Vertex cubeVertices[] = {
    //    { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { {-0.5f,  0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { {-0.5f,  0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f,  0.5f, -0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} },
    //    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.5f, 0.8f, 1.0f} }
    //};


    UINT cubeIndices[] = {
        0, 1, 2,   // Face 1 (clockwise)
        2, 1, 3,   // Face 2 (clockwise)
        4, 6, 5,   // Face 3 (clockwise)
        6, 7, 5,   // Face 4 (clockwise)
        0, 2, 4,   // Face 5 (clockwise)
        2, 6, 4,   // Face 6 (clockwise)
        1, 5, 3,   // Face 7 (clockwise)
        3, 5, 7,   // Face 8 (clockwise)
        2, 3, 6,   // Face 9 (clockwise)
        3, 7, 6,   // Face 10 (clockwise)
        0, 4, 1,   // Face 11 (clockwise)
        1, 4, 5    // Face 12 (clockwise)
    };


    
    const UINT vertexBufferSize = sizeof(cubeVertices);
    const UINT indexBufferSize = sizeof(cubeIndices);



    // Index Buffer
    CD3DX12_HEAP_PROPERTIES heapPropsIndex(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDescIndex = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

    HRESULT hr = renderer->m_pDevice->CreateCommittedResource(
        &heapPropsIndex,
        D3D12_HEAP_FLAG_NONE,
        &bufferDescIndex,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuffer)
    );
    ASSERT_FAILED(hr);

    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readRangeIndex(0, 0);

    hr = indexBuffer->Map(0, &readRangeIndex, reinterpret_cast<void**>(&pIndexDataBegin));
    ASSERT_FAILED(hr);
    memcpy(pIndexDataBegin, cubeIndices, indexBufferSize);
    indexBuffer->Unmap(0, nullptr);

    indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
    indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    indexBufferView.SizeInBytes = indexBufferSize;



    // ** MATRIX

    // Initialisation de la matrice de vue (view)
    XMFLOAT3 eye(0.0f, 0.0f, -2.0f);    // Position de la caméra
    XMFLOAT3 at(0.0f, 0.0f, 0.0f);      // Point où la caméra regarde
    XMFLOAT3 up(0.0f, 1.0f, 0.0f);      // Vecteur "up" (orienté vers le haut)
    XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up));
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
    XMStoreFloat4x4(&m_cbData.view, transposedViewMatrix);

    // Initialisation de la matrice de projection
    float aspectRatio = 16.0f / 9.0f;   // Vous devrez fournir la valeur de l'aspect ratio
    float fieldOfView = XM_PIDIV4;  // Angle de champ de vision (45 degrés ici)
    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, 0.1f, 100.0f);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMStoreFloat4x4(&m_cbData.projection, transposedProjectionMatrix);

    // MATRIX **




    // Vertex Buffer * 
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    hr = renderer->m_pDevice->CreateCommittedResource(
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
    memcpy(pVertexDataBegin, cubeVertices, vertexBufferSize);
    vertexBuffer->Unmap(0, nullptr);

    vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = sizeof(Vertex);
    vertexBufferView.SizeInBytes = vertexBufferSize;



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

    //static float rotationAngle = 0.0f;
    //rotationAngle += 1.0f;
    //m_transformData.Rotate(0, 0, rotationAngle);
    //m_cbData.model = m_transformData.GetTransformMatrix();

    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);


    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = (sizeof(ConstantBufferData) + 255) & ~255; // Alignement sur 256 octets
    renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_pCbvHeap->GetCPUDescriptorHandleForHeapStart());
    // Constant Buffer *

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

    // #TODO Don't repeat command to each triangle


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


    renderer->m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);




    // Rotate
    static float rotationAngle = 0.0f;
    rotationAngle += 1.0f;
    m_transformData.Rotate(0, rotationAngle, 0);
    m_cbData.model = m_transformData.GetTransformMatrix();

    // Map 
    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);
    

    // * Update constant buffer 
    D3D12_GPU_VIRTUAL_ADDRESS cbvAddress = m_constantBuffer->GetGPUVirtualAddress();
    renderer->m_pCommandList->SetGraphicsRootConstantBufferView(0, cbvAddress);
    // Update constant buffer *







    // Record commands.
    const float clearColor[] = { 0.3f, 0.8f, 0.1f, 1.0f };
    renderer->m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    renderer->m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->m_pCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
    renderer->m_pCommandList->IASetIndexBuffer(&indexBufferView);

    renderer->m_pCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);

    CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        renderer->m_pRenderTargets[renderer->m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    renderer->m_pCommandList->ResourceBarrier(1, &presentBarrier);

    hr = renderer->m_pCommandList->Close();  
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
