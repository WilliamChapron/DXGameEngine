#pragma once


#include "Graphics.h"
#include "../core/Defines.h"
#include "../core/Window.h"
#include "../ecs/entities/GameObject.hpp"
#include "../Utils.h"


Renderer::Renderer(Window* pWindow) {

    m_pWindow = pWindow;
    m_frameIndex = 0;
    m_fenceEvent = nullptr;
    m_fenceValue = 0;

    m_pAdapter.Reset();
    m_pDevice.Reset();
    m_pFactory.Reset();
    m_pSwapChain.Reset();
    m_pCommandQueue.Reset();
    m_pCommandAllocator.Reset();
    m_pCommandList.Reset();
    m_pFence.Reset();
    m_pRtvHeap.Reset();
    m_pCbvSrvHeap.Reset();
    m_rtvDescriptorSize = 0;

    for (int i = 0; i < m_FRAME_COUNT; ++i) {
        m_pRenderTargets[i].Reset();
    }



    m_pViewport = new D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<float>(1200), static_cast<float>(900) };
    m_pScissorRect = new D3D12_RECT{ 0, 0, static_cast<LONG>(1200), static_cast<LONG>(900) };
    std::cout << "Init renderer" << std::endl;
}

Renderer::~Renderer() {
    for (int i = 0; i < m_FRAME_COUNT; ++i) {
        m_pRenderTargets[i].Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    }




    m_pRtvHeap.Reset();
    m_pCbvSrvHeap.Reset();
    m_pFence.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    m_pCommandList.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    m_pCommandAllocator.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    m_pCommandQueue.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    m_pSwapChain.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    m_pDevice.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    m_pAdapter.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
    m_pFactory.Reset(); // Utilisation de Reset() pour lib�rer l'interface COM
}

void Renderer::InitializeDirectX12Instances() {

    #if defined(DEBUG) || defined(_DEBUG)
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController))))
        {
            m_debugController->EnableDebugLayer();

            // Enable additional debug layers.
            m_dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    #endif

    CreateFactory();
    CreateDevice();
    CreateCommandQueue();
    CreateSwapChain();
    CreateDescriptorHeap();
    CreateCommandAllocator();
    CreateCommandList();
    CreateFence();


}


void Renderer::CreateSwapChain() {
    // Create the Swap Chain double buffering  

    WindowProperties windowProperties = m_pWindow->getWndProps();

    DXGI_MODE_DESC backBufferDesc = {};
    backBufferDesc.Width = windowProperties.width;
    backBufferDesc.Height = windowProperties.height;
    backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1;


    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = m_FRAME_COUNT;
    swapChainDesc.BufferDesc = backBufferDesc;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = windowProperties.hwnd;
    swapChainDesc.SampleDesc = sampleDesc;
    swapChainDesc.Windowed = true;

    HRESULT hr;

    // Create the swap chain
    hr = m_pFactory->CreateSwapChain(m_pCommandQueue.Get(), &swapChainDesc, &m_pSwapChain);

    m_frameIndex = 0;

    ASSERT_FAILED(hr);

}

void Renderer::CreateFactory() {
    HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&m_pFactory));
    ASSERT_FAILED(hr);
}

void Renderer::CreateDevice() {
    int adapterIndex = 0;
    bool adapterFound = false;

    while (m_pFactory->EnumAdapters1(adapterIndex, &m_pAdapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC1 desc;
        m_pAdapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            adapterIndex++;
            continue;
        }

        HRESULT hr = D3D12CreateDevice(m_pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));
        ASSERT_FAILED(hr);
        if (SUCCEEDED(hr)) {
            adapterFound = true;
            break;  // Sortir de la boucle une fois que l'adaptateur est trouv�
        }

        adapterIndex++;
    }

    if (!adapterFound) {
        PRINT("No adapter found");
        return;
    }

}

void Renderer::CreateCommandQueue() {
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    HRESULT hr = m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));
    ASSERT_FAILED(hr);
}


void Renderer::CreateCommandAllocator() {
    HRESULT hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));
    ASSERT_FAILED(hr);
}

void Renderer::CreateCommandList() {
    HRESULT hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator.Get(), NULL, IID_PPV_ARGS(&m_pCommandList));
    ASSERT_FAILED(hr);
}

void Renderer::CreateFence() {
    HRESULT hr;

    // Create fence
    hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
    ASSERT_FAILED(hr);

    m_fenceValue = 0;

    // Create fence event
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
    {
        HRESULT_FROM_WIN32(GetLastError());
    }
    ASSERT_FAILED(hr);
    PRINT("Fence event success");
}

void Renderer::CreateDescriptorHeap() {
    HRESULT hr;

    // Create RTV descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = m_FRAME_COUNT; // Adjust the number of RTVs as needed
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    hr = m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRtvHeap));
    ASSERT_FAILED(hr);

    m_rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());

    // Create RTV for each frame
    for (int i = 0; i < m_FRAME_COUNT; ++i) {
        hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRenderTargets[i]));
        ASSERT_FAILED(hr);
        m_pDevice->CreateRenderTargetView(m_pRenderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);
    }

    // Create CBV / SRV / UAV descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
    cbvHeapDesc.NumDescriptors = 100;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    hr = m_pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_pCbvSrvHeap));
    ASSERT_FAILED(hr);

    m_cbvSrvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}





void Renderer::WaitForPreviousFrame()
{
    //PRINT("Waiting for previous frame...");

    // Signal and increment the fence value.
    m_fenceValue++;

    HRESULT hr = m_pCommandQueue->Signal(m_pFence.Get(), m_fenceValue);
    ASSERT_FAILED(hr);

    // Wait until the previous frame is finished.
    if (m_pFence->GetCompletedValue() < m_fenceValue)
    {
        //PRINT("RENTRE DANS PREVIOUS FRAME");
        //Check if the GPU has completed all commands associated with the previous fence value
        HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);

        //Set the event to the current fence value
        hr = m_pFence->SetEventOnCompletion(m_fenceValue, eventHandle);
        ASSERT_FAILED(hr);

        //Wait for the GPU to complete associated commands
        WaitForSingleObject(eventHandle, INFINITE);

        //Close the handle to the event
        CloseHandle(eventHandle);
    }



}




// # TODO | SEPARER LES TACHES DE LA COMMAND LIST POUR POUVOIR UPDATE LE COMPONENT SHADER DE CHAQUE GAME OBJECT AU BON MOMENT DANS LE GAME OBJECT MANAGER
void Renderer::Precommandlist() {

    HRESULT hr;

    hr = m_pCommandAllocator->Reset();
    ASSERT_FAILED(hr);

    hr = m_pCommandList->Reset(m_pCommandAllocator.Get(), nullptr);
    ASSERT_FAILED(hr);

    CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_pRenderTargets[m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_pCommandList->ResourceBarrier(1, &transitionBarrier);

    ID3D12DescriptorHeap* heaps[] = { m_pCbvSrvHeap.Get() };
    m_pCommandList->SetDescriptorHeaps(_countof(heaps), heaps);

    m_pCommandList->RSSetViewports(1, m_pViewport);
    m_pCommandList->RSSetScissorRects(1, m_pScissorRect);

    //m_pCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());
    //m_pCommandList->SetPipelineState(m_pPipelineState.Get());



    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
        m_frameIndex,
        m_rtvDescriptorSize);
    m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);


    const float clearColor[] = { 0.18f, 0.5f, 0.18f, 0.0f };
    m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

}

void Renderer::Postcommandlist()
{

    HRESULT hr;

    CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_pRenderTargets[m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    m_pCommandList->ResourceBarrier(1, &presentBarrier);

    hr = m_pCommandList->Close();
    ASSERT_FAILED(hr);

}


//D3D12_RASTERIZER_DESC rasterizerStateDesc {};
//rasterizerStateDesc.FillMode = D3D12_FILL_MODE_SOLID;   // Remplissage solide
//rasterizerStateDesc.CullMode = D3D12_CULL_MODE_FRONT;    // D�sactivation du culling
//rasterizerStateDesc.FrontCounterClockwise = TRUE;       // Les triangles sont d�finis dans le sens inverse des aiguilles d'une montre (orientation des sommets)
//rasterizerStateDesc.DepthBias = 0;
//rasterizerStateDesc.DepthBiasClamp = 0.0f;
//rasterizerStateDesc.SlopeScaledDepthBias = 0;
//rasterizerStateDesc.DepthClipEnable = FALSE;             // Activation du test de profondeur

//rasterizerStateDesc.MultisampleEnable = FALSE;          // D�sactivation de l'�chantillonnage multiple
//rasterizerStateDesc.AntialiasedLineEnable = FALSE;

//D3D12_BLEND_DESC blendDesc {};