#include "Texture.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../../DDSTextureLoader.h"
#include "../../Utils.h"

TextureComponent::TextureComponent(std::string name) : Component(name, ComponentType::Texture)
{

}

void TextureComponent::Initialize(Renderer* renderer, int offset)
{
    HRESULT hr;

    m_offset = offset - 1;




    // Reset Alloc / List
    hr = renderer->m_pCommandAllocator->Reset();
    ASSERT_FAILED(hr);
    hr = renderer->m_pCommandList->Reset(renderer->m_pCommandAllocator.Get(), nullptr);
    ASSERT_FAILED(hr);


    std::wstring texturePath = L"res/texture/" + stringToWString(GetName()) + L".dds";

    // Create / Upload Ressource With Command List
    hr = CreateDDSTextureFromFile12(renderer->m_pDevice.Get(),
        renderer->m_pCommandList.Get(),
        texturePath.c_str(),
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

  
    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
    srvHandle.Offset(m_offset, renderer->m_cbvSrvDescriptorSize);


    // PEUT ON RECUPERER LA

    hr = renderer->m_pCommandList->Close();
    ASSERT_FAILED(hr);

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    renderer->WaitForPreviousFrame();

    renderer->m_pDevice->CreateShaderResourceView(m_textureBuffer.Get(), &srvDesc, srvHandle);

    // Get List from DDS Creation


    m_textureAdress = CD3DX12_GPU_DESCRIPTOR_HANDLE(renderer->m_pCbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
    m_textureAdress.Offset(m_offset, renderer->m_cbvSrvDescriptorSize);
    
}

void TextureComponent::Update(Renderer* renderer) {
    //PRINT("Update Texture");
    //PRINT("OFFSET");
    //PRINT(m_offset);
    //CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvHandle(renderer->m_pCbvSrvHeap.Get()->GetGPUDescriptorHandleForHeapStart());
    //cbvSrvHandle.Offset(m_offset, renderer->m_cbvSrvDescriptorSize); // Utilisation du bon increment size
    renderer->m_pCommandList->SetGraphicsRootDescriptorTable(0, m_textureAdress);



}