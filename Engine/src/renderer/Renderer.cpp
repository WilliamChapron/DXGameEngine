#pragma once

#include "../include.h"
#include "Renderer.h"
#include "../core/Defines.h"
#include "../core/Window.h"

Renderer::Renderer() {
    std::cout << "Init renderer" << std::endl;
}

void Renderer::InitializeDirectX12Instances() {
    CreateFactory();
    CreateDevice();
    CreateCommandQueue();
    CreateSwapChain();
    CreateCommandAllocator();
    CreateCommandList();
}

void Renderer::InitializeRessources() {
    CreateShaders();
}

void Renderer::CreateSwapChain() {
    // Create the Swap Chain double buffering  

    DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
    backBufferDesc.Width = Window::GetInstance().getWndProps().width; // buffer width
    backBufferDesc.Height = Window::GetInstance().getWndProps().height; // buffer height
    backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each channel)

    // describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2; // number of buffers we have
    swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
    swapChainDesc.OutputWindow = Window::GetInstance().getWndProps().hwnd; // handle to our window
    swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
    swapChainDesc.Windowed = true; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

    HRESULT hr;

    // Create the swap chain
    hr = pFactory->CreateSwapChain(pCommandQueue, &swapChainDesc, &pSwapChain);

    frameIndex = 0;

    if (CHECK_SUCCESS(hr, pSwapChain)) {
        LOG_SUCCESS("Swap chain", "create");
    }

    if (CHECK_FAILURE(hr, pSwapChain)) {
        LOG_FAILURE("Swap chain", "create");
        return;  
    }

}

void Renderer::CreateFactory() {
    HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
    if (CHECK_SUCCESS(hr, pFactory)) {
        LOG_SUCCESS("DXGI Factory", "create");
    }
    else {
        LOG_FAILURE("DXGI Factory", "create");
        return; 
    }
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

        HRESULT hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice));
        if (CHECK_SUCCESS(hr, pDevice)) {
            LOG_SUCCESS("DXGI Device", "create");
            PRINT("Adapter found");
            adapterFound = true;
            break;
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
    if (CHECK_SUCCESS(hr, pCommandQueue)) {
        LOG_SUCCESS("Command queue", "create");
    }
    else {
        LOG_FAILURE("Command queue", "create");
        return;
    }
}


void Renderer::CreateCommandAllocator() {
    HRESULT hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAllocator));
    if (CHECK_SUCCESS(hr, pCommandAllocator)) {
        LOG_SUCCESS("Command allocator", "create");
    }
    else {
        LOG_FAILURE("Command allocator", "create");
        return;
    }
}

void Renderer::CreateCommandList() {
    HRESULT hr = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCommandAllocator, NULL, IID_PPV_ARGS(&pCommandList));
    if (CHECK_SUCCESS(hr, pCommandList)) {
        LOG_SUCCESS("Command list", "create");
    }
    else {
        LOG_FAILURE("Command list", "create");
        return;
    }
}

void Renderer::CreateFence() {
    HRESULT hr;

    // Create fence
    hr = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence));
    if (CHECK_SUCCESS(hr, pFence)) {
        LOG_SUCCESS("Fence", "create");
    }

    if (CHECK_FAILURE(hr, pFence)) {
        LOG_FAILURE("Fence", "create");
        return;  
    }

    fenceValue = 1;

    // Create fence event
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent == nullptr) {
        // Handle event creation failure
        LOG_FAILURE("Fence Event", "create");
        return; 
    }
}

void Renderer::CreateDescriptorHeap() {
    HRESULT hr;

    // Create descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = 1; // Adjust the number of descriptors as needed
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // Adjust the type based on your requirements
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    hr = pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pDescriptorHeap));
    if (CHECK_SUCCESS(hr, pDescriptorHeap)) {
        LOG_SUCCESS("Descriptor Heap", "create");
    }

    if (CHECK_FAILURE(hr, pDescriptorHeap)) {
        LOG_FAILURE("Descriptor Heap", "create");
        // Handle failure if needed
        return;  // Stop further initialization on failure
    }
}



void Renderer::CreatePipelineState() {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };

    HRESULT hr = pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState));
}


void Renderer::CreateRootSignature() {
    // Définir les paramètres de la root signature
    CD3DX12_ROOT_PARAMETER rootParameters[1];
    rootParameters[0].InitAsConstantBufferView(0);
    //

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(1, rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Serialize
    ID3DBlob* signature = nullptr;
    ID3DBlob* error = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);


    hr = pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
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
        return hr;
    }

    if (errorBlob)
        errorBlob->Release();


    PRINT("OK");

    return S_OK;
}

void Renderer::CreateShaders() {
    ID3DBlob* vertexShaderBlob = nullptr;
    ID3DBlob* pixelShaderBlob = nullptr;

    CompileShaderFromFile(L"res/shader/VS.hlsl", "main", "vs_5_0", &vertexShaderBlob);
    CompileShaderFromFile(L"res/shader/PS.hlsl", "main", "ps_5_0", &pixelShaderBlob);
}

