#pragma once

 
#include "../include.h"   
#include "Renderer.h"   
#include "../core/Defines.h"
#include "../core/Window.h"

Renderer::Renderer() {
    std::cout << "Init renderer" << std::endl;
}

void Renderer::CreateSwapChain() {
    // -- Create the Swap Chain (double/tripple buffering) -- //

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
    swapChainDesc.Windowed = false; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

    HRESULT hr;

    // Create the swap chain
    hr = pFactory->CreateSwapChain(pCommandQueue, &swapChainDesc, &pSwapChain);

    frameIndex = 0;

    if (CHECK_SUCCESS(hr, pSwapChain)) {
        LOG_SUCCESS("Swap chain", "create");
    }

    if (CHECK_FAILURE(hr, pSwapChain)) {
        LOG_FAILURE("Swap chain", "create");
        // Handle failure if needed
        return;  // Stop further initialization on failure
    }

}

void Renderer::InitializeDirectX12Instances() {
    std::cout << "InitializeDirectX12Instances" << std::endl;

    HRESULT hr;

    // Create com factory
    hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
    if (CHECK_SUCCESS(hr, pFactory)) {
        LOG_SUCCESS("DXGI Factory", "create");
    }

    if (CHECK_FAILURE(hr, pFactory)) {
        LOG_FAILURE("DXGI Factory", "create");
        // Handle failure if needed
        return;  // Stop further initialization on failure
    }

    // -- Create the Device -- //

    int adapterIndex = 0;
    bool adapterFound = false;

    // Find first hardware GPU that supports D3D 12
    while (pFactory->EnumAdapters1(adapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC1 desc;
        pAdapter->GetDesc1(&desc);
        PRINT(adapterIndex);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            // we dont want a software device
            adapterIndex++; // add this line here. Its not currently in the downloadable project
            continue;
        }

        // we want a device that is compatible with direct3d 12 (feature level 11 or higher)
        hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice));
        if (CHECK_SUCCESS(hr, pDevice)) {
            LOG_SUCCESS("DXGI Device", "create");
            PRINT("Adapter found");
            adapterFound = true;
            break;
        }
        adapterIndex++;
    }

    if (!adapterFound)
    {
        PRINT("No adapter found");
        return;
    }

     // Create command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};  // Add your command queue description here
    hr = pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pCommandQueue));
    if (CHECK_SUCCESS(hr, pCommandQueue)) {
        LOG_SUCCESS("Command queue", "create");
    }

    if (CHECK_FAILURE(hr, pCommandQueue)) {
        LOG_FAILURE("Command queue", "create");
        return; 
    }


    


    // Create swap chain
    CreateSwapChain();
    

    


    // Create command allocator
    hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAllocator));
    if (CHECK_SUCCESS(hr, pCommandAllocator)) {
        LOG_SUCCESS("Command allocator", "create");
    }

    if (CHECK_FAILURE(hr, pCommandAllocator)) {
        LOG_FAILURE("Command allocator", "create");
        // Handle failure if needed
        return;  // Stop further initialization on failure
    }

}