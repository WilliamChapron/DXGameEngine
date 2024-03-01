#pragma once

#include "../include.h"
#include "Resources.h"
#include <iostream>
#include <vector>


class Window;
class GameObject;

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Renderer {
public:
    Renderer(Window* pWindow);
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



    void WaitForPreviousFrame();


    void Precommandlist();
    void Postcommandlist();

    static const UINT m_FRAME_COUNT = 2;
    int m_frameIndex;
    HANDLE m_fenceEvent;
    UINT64 m_fenceValue;

    ComPtr<IDXGIAdapter1> m_pAdapter;
    ComPtr<ID3D12Device> m_pDevice;
    ComPtr<IDXGIFactory4> m_pFactory;
    ComPtr<IDXGISwapChain> m_pSwapChain;
    ComPtr<ID3D12CommandQueue> m_pCommandQueue;
    ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
    ComPtr<ID3D12Fence> m_pFence;



    ComPtr<ID3D12Resource> m_pRenderTargets[m_FRAME_COUNT];

    D3D12_VIEWPORT* m_pViewport;
    D3D12_RECT* m_pScissorRect;

    ComPtr<ID3DBlob> m_vertexShaderBlob;
    ComPtr<ID3DBlob> m_pixelShaderBlob;

    ComPtr<ID3D12PipelineState> m_pPipelineState;
    ComPtr<ID3D12RootSignature> m_pRootSignature;

    ComPtr<ID3D12Debug> m_debugController;
    UINT m_dxgiFactoryFlags = 0;

    Window* m_pWindow;

    // HEAP
    ComPtr<ID3D12DescriptorHeap> m_pRtvHeap; 
    UINT m_rtvDescriptorSize;
    ComPtr<ID3D12DescriptorHeap> m_pCbvSrvHeap; 
    UINT m_cbvSrvDescriptorSize;

    int m_renderCallNum = 0;




private:


};
