#include "Texture.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../../DDSTextureLoader.h"

Texture::Texture(std::string name) : name(name)
{
}

void Texture::Initialize(Renderer* renderer)
{
    HRESULT hr;

    // Reset Alloc / List
    hr = renderer->m_pCommandAllocator->Reset();
    ASSERT_FAILED(hr);
    hr = renderer->m_pCommandList->Reset(renderer->m_pCommandAllocator.Get(), nullptr);
    ASSERT_FAILED(hr);

    // Create / Upload Ressource With Command List
    hr = CreateDDSTextureFromFile12(renderer->m_pDevice.Get(),
        renderer->m_pCommandList.Get(),
        L"res/texture/texture.dds",
        m_textureBuffer,
        m_uploadTexture,
        0,
        nullptr
    );
    ASSERT_FAILED(hr);



    // Create S R V
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = m_textureBuffer->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = m_textureBuffer->GetDesc().MipLevels;
    renderer->m_pDevice->CreateShaderResourceView(m_textureBuffer.Get(), &srvDesc, renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());

    // Get List from DDS Creation

    hr = renderer->m_pCommandList->Close();
    ASSERT_FAILED(hr);

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Wait Fence / #THINK Init Value can Impact ??
    renderer->WaitForPreviousFrame();

    PRINT(m_textureBuffer.Get());
    PRINT(m_uploadTexture.Get());


}
