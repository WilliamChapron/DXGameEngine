#pragma once

#include "../include.h"
#include "Resources.h"
#include <iostream>
#include <vector>
#include <wrl/client.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Renderer {
public:
    Renderer();
    ~Renderer();

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

    static const UINT FRAME_COUNT = 2;
    int frameIndex;
    HANDLE fenceEvent;
    UINT64 fenceValue;

    ComPtr<IDXGIAdapter1> pAdapter;
    ComPtr<ID3D12Device> pDevice;
    ComPtr<IDXGIFactory4> pFactory;
    ComPtr<IDXGISwapChain> pSwapChain;
    ComPtr<ID3D12CommandQueue> pCommandQueue;
    ComPtr<ID3D12CommandAllocator> pCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> pCommandList;
    ComPtr<ID3D12Fence> pFence;
    ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
    UINT rtvDescriptorSize;

    ComPtr<ID3D12Resource> pRenderTargets[FRAME_COUNT];

    D3D12_VIEWPORT* pViewport;
    D3D12_RECT* pScissorRect;

    ComPtr<ID3DBlob> vertexShaderBlob;
    ComPtr<ID3DBlob> pixelShaderBlob;

    ComPtr<ID3D12PipelineState> pPipelineState;
    ComPtr<ID3D12RootSignature> pRootSignature;

    ComPtr<ID3D12Debug> debugController;
    UINT dxgiFactoryFlags = 0;




private:


};
