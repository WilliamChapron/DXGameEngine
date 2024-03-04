#pragma once

#include "../include.h"
#include "Graphics.h"
#include "../core/Defines.h"
#include "../core/Window.h"
#include "../ecs/entities/GameObject.h"


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

    m_vertexShaderBlob.Reset();
    m_pixelShaderBlob.Reset();

    for (int i = 0; i < m_FRAME_COUNT; ++i) {
        m_pRenderTargets[i].Reset();
    }

    m_pPipelineState.Reset();
    m_pRootSignature.Reset();

    m_pViewport = new D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<float>(1200), static_cast<float>(900) };
    m_pScissorRect = new D3D12_RECT{ 0, 0, static_cast<LONG>(1200), static_cast<LONG>(900) };
    std::cout << "Init renderer" << std::endl;
}

Renderer::~Renderer() {
    for (int i = 0; i < m_FRAME_COUNT; ++i) {
        m_pRenderTargets[i].Reset(); // Utilisation de Reset() pour libérer l'interface COM
    }


    m_pPipelineState.Reset(); 
    m_pRootSignature.Reset(); 

    m_vertexShaderBlob.Reset(); 
    m_pixelShaderBlob.Reset(); 

    m_pRtvHeap.Reset(); 
    m_pCbvSrvHeap.Reset();
    m_pFence.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    m_pCommandList.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    m_pCommandAllocator.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    m_pCommandQueue.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    m_pSwapChain.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    m_pDevice.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    m_pAdapter.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    m_pFactory.Reset(); // Utilisation de Reset() pour libérer l'interface COM
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

    
    CreateRootSignature();
    CreatePipelineState();

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
            break;  // Sortir de la boucle une fois que l'adaptateur est trouvé
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


HRESULT CompileShaderFromFile(const wchar_t* filePath, const char* entryPoint, const char* shaderModel, ID3DBlob** blob)
{
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

    #if defined(DEBUG) || defined(_DEBUG)
        shaderFlags |= D3DCOMPILE_DEBUG;
    #endif

    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, blob, &errorBlob);
    ASSERT_FAILED(hr);

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(static_cast<const char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
        PRINT("Shader compilation failed");
        return hr;
    }

    if (errorBlob) errorBlob->Release();

    PRINT("Shader compilation successful");

    return S_OK;
}

void Renderer::CreateRootSignature() {

    CD3DX12_DESCRIPTOR_RANGE range;
    range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
        
    CD3DX12_ROOT_PARAMETER parameter[2];
    parameter[0].InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_ALL);
    parameter[1].InitAsConstantBufferView(0); //b0



    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | 
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;

    CD3DX12_STATIC_SAMPLER_DESC sampler(
        0,
        D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressW
        0.0f,
        1,
        D3D12_COMPARISON_FUNC_ALWAYS,
        D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
        0.0f,
        D3D12_FLOAT32_MAX,
        D3D12_SHADER_VISIBILITY_ALL
    );

    rootSignatureDesc.Init(_countof(parameter), parameter, 1, &sampler, rootSignatureFlags);

    ID3DBlob* signature = nullptr;
    ID3DBlob* error = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    ASSERT_FAILED(hr);
    hr = m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));
    ASSERT_FAILED(hr);
    PRINT("Root Signature successfully");
}



void Renderer::CreatePipelineState() {

    #if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        UINT compileFlags = 0;
    #endif
    // Charger les shaders spécifiques au Triangle
    HRESULT hr = CompileShaderFromFile(L"res/shader/Shader.hlsl", "VSMain", "vs_5_0", &m_vertexShaderBlob);
    ASSERT_FAILED(hr);
    hr = CompileShaderFromFile(L"res/shader/Shader.hlsl", "PSMain", "ps_5_0", &m_pixelShaderBlob);
    ASSERT_FAILED(hr);
    PRINT("Shaders loaded successfully");

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc; // a structure to define a pso
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)); // IMPORTANT ?
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = m_pRootSignature.Get();
    psoDesc.VS = { m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize() };
    psoDesc.PS = { m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize() }; // Use the filled bytecode structure
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the render target
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = 0xffffffff;
    psoDesc.NumRenderTargets = 1;
    psoDesc.SampleDesc.Count = 1;

    hr = m_pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
    ASSERT_FAILED(hr);

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

    m_pCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());
    m_pCommandList->SetPipelineState(m_pPipelineState.Get());



    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
        m_frameIndex,
        m_rtvDescriptorSize);
    m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);


    const float clearColor[] = { 0.18f, 0.18f, 0.18f, 0.0f };
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
//rasterizerStateDesc.CullMode = D3D12_CULL_MODE_FRONT;    // Désactivation du culling
//rasterizerStateDesc.FrontCounterClockwise = TRUE;       // Les triangles sont définis dans le sens inverse des aiguilles d'une montre (orientation des sommets)
//rasterizerStateDesc.DepthBias = 0;
//rasterizerStateDesc.DepthBiasClamp = 0.0f;
//rasterizerStateDesc.SlopeScaledDepthBias = 0;
//rasterizerStateDesc.DepthClipEnable = FALSE;             // Activation du test de profondeur

//rasterizerStateDesc.MultisampleEnable = FALSE;          // Désactivation de l'échantillonnage multiple
//rasterizerStateDesc.AntialiasedLineEnable = FALSE;

//D3D12_BLEND_DESC blendDesc {};