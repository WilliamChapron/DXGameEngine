#pragma once

#include "../include.h"
#include "Resources.h"
#include <iostream>
#include <vector>

using namespace DirectX;

class Renderer {
public:
    Renderer();
    //~Renderer();

    void InitializeDirectX12Instances();
    void CreateFactory();
    void CreateSwapChain();
    void CreateDevice();
    void CreateCommandQueue();
    void CreateCommandAllocator();
    void CreateCommandList();
    void CreateFence();
    void CreateDescriptorHeap();
    void CreatePipelineState();
    void CreateRootSignature();

    //void Render();  // Ajoutez cette fonction pour rendre votre triangle

private:
    IDXGIAdapter1* pAdapter;
    ID3D12Device* pDevice = nullptr;
    IDXGIFactory4* pFactory = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D12CommandQueue* pCommandQueue = nullptr;
    ID3D12CommandAllocator* pCommandAllocator = nullptr;
    ID3D12GraphicsCommandList* pCommandList = nullptr;
    ID3D12Fence* pFence = nullptr;
    ID3D12DescriptorHeap* pDescriptorHeap = nullptr;

    // 

    ID3D12PipelineState* pPipelineState;
    ID3D12RootSignature* pRootSignature;

    const int FRAME_COUNT = 2;
    int frameIndex = 0;
    HANDLE fenceEvent = nullptr;
    UINT64 fenceValue = 0;
};
