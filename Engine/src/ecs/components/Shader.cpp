#include "Shader.h"
#include "../../renderer/Graphics.h" 
#include "../../core/Defines.h"
#include "../../Utils.h"

ShaderComponent::ShaderComponent(std::string name, Renderer* pRenderer) : Component(name, ComponentType::Shader), m_pRenderer(pRenderer)
{
}

ShaderComponent::~ShaderComponent() {
    m_pPipelineState.Reset();
    m_pRootSignature.Reset();

    m_vertexShaderBlob.Reset();
    m_pixelShaderBlob.Reset();
}

void ShaderComponent::InitializeRootSignature() {



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
    hr = m_pRenderer->m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));
    ASSERT_FAILED(hr);
    PRINT("Root Signature successfully");
}



void ShaderComponent::InitializePSO() {



    #if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        UINT compileFlags = 0;
    #endif


    HRESULT hr = CompileShaderFromFile(L"../Engine/res/shader/Shader.hlsl", "VSMain", "vs_5_0", &m_vertexShaderBlob);
    ASSERT_FAILED(hr);
    hr = CompileShaderFromFile(L"../Engine/res/shader/Shader.hlsl", "PSMain", "ps_5_0", &m_pixelShaderBlob);
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

    hr = m_pRenderer->m_pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
    ASSERT_FAILED(hr);

}




void ShaderComponent::Update(Renderer* pRenderer)
{
    PRINT("Update Shader");
    UpdatePSO();
    UpdateRootSignature();
}

void ShaderComponent::UpdatePSO()
{

     m_pRenderer->m_pCommandList->SetPipelineState(m_pPipelineState.Get());
}

void ShaderComponent::UpdateRootSignature()
{
    m_pRenderer->m_pCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());
}
