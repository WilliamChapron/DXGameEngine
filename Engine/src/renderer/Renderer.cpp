#pragma once

#include "../include.h"
#include "Renderer.h"
#include "../core/Defines.h"
#include "../core/Window.h"


Renderer::Renderer() {
    std::cout << "Init renderer" << std::endl;
}

Renderer::~Renderer() {
    for (int i = 0; i < FRAME_COUNT; ++i) {
        pRenderTargets[i].Reset(); // Utilisation de Reset() pour libérer l'interface COM
    }


    pPipelineState.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pRootSignature.Reset(); // Utilisation de Reset() pour libérer l'interface COM

    vertexShaderBlob.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pixelShaderBlob.Reset(); // Utilisation de Reset() pour libérer l'interface COM

    pDescriptorHeap.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pFence.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pCommandList.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pCommandAllocator.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pCommandQueue.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pSwapChain.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pDevice.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pAdapter.Reset(); // Utilisation de Reset() pour libérer l'interface COM
    pFactory.Reset(); // Utilisation de Reset() pour libérer l'interface COM
}

void Renderer::InitializeDirectX12Instances() {

    #if defined(DEBUG) || defined(_DEBUG)
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    #endif


    frameIndex = 0;
    fenceEvent = nullptr;
    fenceValue = 0;

    pAdapter.Reset();
    pDevice.Reset();
    pFactory.Reset();
    pSwapChain.Reset();
    pCommandQueue.Reset();
    pCommandAllocator.Reset();
    pCommandList.Reset();
    pFence.Reset();
    pDescriptorHeap.Reset();
    rtvDescriptorSize = 0;

    vertexShaderBlob.Reset();
    pixelShaderBlob.Reset();

    for (int i = 0; i < FRAME_COUNT; ++i) {
        pRenderTargets[i].Reset();
    }



    pPipelineState.Reset();
    pRootSignature.Reset();

    pViewport = new D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<float>(1200), static_cast<float>(900) };
    pScissorRect = new D3D12_RECT{ 0, 0, static_cast<LONG>(1200), static_cast<LONG>(900) };

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

    pCommandList->Close();
}


void Renderer::CreateSwapChain() {
    // Create the Swap Chain double buffering  

    DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
    backBufferDesc.Width = Window::GetInstance().getWndProps().width; // buffer width
    backBufferDesc.Height = Window::GetInstance().getWndProps().height; // buffer height
    backBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // format of the buffer (rgba 32 bits, 8 bits for each channel)

    // describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_COUNT; // number of buffers we have
    swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
    swapChainDesc.OutputWindow = Window::GetInstance().getWndProps().hwnd; // handle to our window
    swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
    swapChainDesc.Windowed = true; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

    HRESULT hr;

    // Create the swap chain
    hr = pFactory->CreateSwapChain(pCommandQueue.Get(), &swapChainDesc, &pSwapChain);

    frameIndex = 0;

    ASSERT_FAILED(hr);

}

void Renderer::CreateFactory() {
    HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
    ASSERT_FAILED(hr);
}

void Renderer::CreateDevice() {
    int adapterIndex = 0;
    bool adapterFound = false;

    while (pFactory->EnumAdapters1(adapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC1 desc;
        pAdapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            adapterIndex++;
            continue;
        }

        HRESULT hr = D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice));
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
    HRESULT hr = pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pCommandQueue));
    ASSERT_FAILED(hr);
}


void Renderer::CreateCommandAllocator() {
    HRESULT hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAllocator));
    ASSERT_FAILED(hr);
}

void Renderer::CreateCommandList() {
    HRESULT hr = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCommandAllocator.Get(), NULL, IID_PPV_ARGS(&pCommandList));
    ASSERT_FAILED(hr);
}

void Renderer::CreateFence() {
    HRESULT hr;

    // Create fence
    hr = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence));
    ASSERT_FAILED(hr);

    fenceValue = 1;

    // Create fence event
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    ASSERT_FAILED(hr);
    PRINT("Fence event success");
}

void Renderer::CreateDescriptorHeap() {
    HRESULT hr;

    // Create descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = FRAME_COUNT; // Adjust the number of descriptors as needed
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // Adjust the type based on your requirements
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    hr = pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pDescriptorHeap));
    ASSERT_FAILED(hr);


    rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    // Create a RTV for each frame.
    for (int i = 0; i < FRAME_COUNT; ++i) {
        HRESULT hr = pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pRenderTargets[i]));
        ASSERT_FAILED(hr);
        pDevice->CreateRenderTargetView(pRenderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, rtvDescriptorSize);
    }
}

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
        PRINT("Shader compilation failed");
        return hr;
    }

    if (errorBlob)
        errorBlob->Release();

    PRINT("Shader compilation successful");

    return S_OK;
}

void Renderer::CreateRootSignature() {
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.pParameters = nullptr;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;

    ID3DBlob* signature = nullptr;
    ID3DBlob* error = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    ASSERT_FAILED(hr);
    hr = pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
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
    HRESULT hr = CompileShaderFromFile(L"res/shader/VS.hlsl", "VSMain", "vs_5_0", &vertexShaderBlob);
    ASSERT_FAILED(hr);
    hr = CompileShaderFromFile(L"res/shader/PS.hlsl", "PSMain", "ps_5_0", &pixelShaderBlob); 
    ASSERT_FAILED(hr);
    PRINT("Shaders loaded successfully");

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    //// Create our vertex input layout
    //D3D12_INPUT_ELEMENT_DESC layout[2];
    //layout[0].SemanticName = "POSITION";
    //layout[0].SemanticIndex = 0;
    //layout[0].Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // 8 bytes
    //layout[0].InputSlot = 0;
    //layout[0].AlignedByteOffset = 0;
    //layout[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    //layout[0].InstanceDataStepRate = 0;

    //layout[1].SemanticName = "COLOR";
    //layout[1].SemanticIndex = 0;
    //layout[1].Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // 8 bytes
    //layout[1].InputSlot = 0;
    //layout[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    //layout[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    //layout[1].InstanceDataStepRate = 0;


    D3D12_RASTERIZER_DESC rasterizerStateDesc;
    ZeroMemory(&rasterizerStateDesc, sizeof(D3D12_RASTERIZER_DESC));

    rasterizerStateDesc.FillMode = D3D12_FILL_MODE_SOLID;   // Remplissage solide
    rasterizerStateDesc.CullMode = D3D12_CULL_MODE_FRONT;   // Culling du côté avant
    rasterizerStateDesc.FrontCounterClockwise = TRUE;       // Les triangles sont définis dans le sens inverse des aiguilles d'une montre (orientation des sommets)
    rasterizerStateDesc.DepthBias = 0;
    rasterizerStateDesc.DepthBiasClamp = 0.0f;
    rasterizerStateDesc.SlopeScaledDepthBias = 0;
    rasterizerStateDesc.DepthClipEnable = TRUE;             // Activation du test de profondeur

    rasterizerStateDesc.MultisampleEnable = FALSE;          // Désactivation de l'échantillonnage multiple
    rasterizerStateDesc.AntialiasedLineEnable = FALSE;

    D3D12_BLEND_DESC blendDesc{};

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc; // a structure to define a pso
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC)); // IMPORTANT ?
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = pRootSignature.Get();
    psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
    psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() }; // Use the filled bytecode structure
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT; // format of the render target
    psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
    psoDesc.RasterizerState = rasterizerStateDesc; // a default rasterizer state.
    psoDesc.BlendState = blendDesc; // a default blent state.
    psoDesc.NumRenderTargets = 2; // we are only binding one render target
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    hr = pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState));
    ASSERT_FAILED(hr);


    //D3D12_RESOURCE_DESC pipelineFormat = pPipelineState->GetDevice();

}



