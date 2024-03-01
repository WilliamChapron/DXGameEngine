

#include "Triangle.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include <stdexcept>

#include "../../Utils.h"
#include <DirectXColors.h>

#include "../components/Camera.h"

#include "../../renderer/Resources.h"
#include "../../DDSTextureLoader.h"



//static const UINT TextureWidth = 256;
//static const UINT TextureHeight = 256;
//static const UINT TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.
//
//
//std::vector<UINT8> GenerateTextureData()
//{
//    const UINT rowPitch = TextureWidth * TexturePixelSize;
//    const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
//    const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
//    const UINT textureSize = rowPitch * TextureHeight;
//
//    std::vector<UINT8> data(textureSize);
//    UINT8* pData = &data[0];
//
//    for (UINT n = 0; n < textureSize; n += TexturePixelSize)
//    {
//        UINT x = n % rowPitch;
//        UINT y = n / rowPitch;
//        UINT i = x / cellPitch;
//        UINT j = y / cellHeight;
//
//        if (i % 2 == j % 2)
//        {
//            pData[n] = 0x00;        // R
//            pData[n + 1] = 0x00;    // G
//            pData[n + 2] = 0x00;    // B
//            pData[n + 3] = 0xff;    // A
//        }
//        else
//        {
//            pData[n] = 0xff;        // R
//            pData[n + 1] = 0xff;    // G
//            pData[n + 2] = 0xff;    // B
//            pData[n + 3] = 0xff;    // A
//        }
//    }
//
//    return data;
//}


Triangle::Triangle() : GameObject()
{
}

Triangle::~Triangle() {

    if (m_vertexBuffer != nullptr) {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }
}


void Triangle::Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) {

    HRESULT hr;


    m_transform = Transform(position, rotation, scale);
    m_cbData.model = m_transform.GetTransformMatrix();

    // ** MATRIX

    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();

    // MATRIX **

    Vertex cubeVertices[] = {
        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  // Rouge
        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  // Rouge
        { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },  // Bleu
        { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },  // Bleu
        { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },  // Jaune
        { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },  // Jaune
        { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },  // Gris
        { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }   // Gris
    };

    UINT cubeIndices[] = {
        0, 1, 2,   // Face 1 (clockwise)
        2, 1, 3,   // Face 2 (clockwise)
        4, 6, 5,   // Face 3 (clockwise)
        6, 7, 5,   // Face 4 (clockwise)
        0, 2, 4,   // Face 5 (clockwise)
        2, 6, 4,   // Face 6 (clockwise)
        1, 5, 3,   // Face 7 (clockwise)
        3, 5, 7,   // Face 8 (clockwise)
        2, 3, 6,   // Face 9 (clockwise)
        3, 7, 6,   // Face 10 (clockwise)
        0, 4, 1,   // Face 11 (clockwise)
        1, 4, 5    // Face 12 (clockwise)
    };

    const UINT vertexBufferSize = sizeof(cubeVertices);// *sizeof(Vertex);
    const UINT indexBufferSize = sizeof(cubeIndices);// *sizeof(UINT);
    const UINT stride = sizeof(Vertex);


    CreateIndexBuffer(indexBufferSize, cubeIndices, m_indexBuffer, m_indexBufferView, renderer);
    CreateVertexBuffer(vertexBufferSize, cubeVertices, m_vertexBuffer, m_vertexBufferView, stride, renderer);

    // * Constant Buffer
    CD3DX12_HEAP_PROPERTIES cbHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC cbDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstantBufferData) + 255) & ~255);

    hr = renderer->m_pDevice->CreateCommittedResource(
        &cbHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &cbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)
    );
    ASSERT_FAILED(hr);

    hr = m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantBuffer));
    ASSERT_FAILED(hr);
    
    memcpy(m_mappedConstantBuffer, &m_cbData, sizeof(ConstantBufferData));
    m_constantBuffer->Unmap(0, nullptr);


    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = (sizeof(ConstantBufferData) + 255) & ~255; // Alignement sur 256 octets
    renderer->m_pDevice->CreateConstantBufferView(&cbvDesc, renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
    // Constant Buffer *


    //ComPtr<int> dd;

    hr = CreateDDSTextureFromFile12(renderer->m_pDevice.Get(),
        renderer->m_pCommandList.Get(),
        L"chemin/vers/votre/fichier.dds",
        m_textureBuffer,
        m_uploadTexture,
        0,
        nullptr
    );
    ASSERT_FAILED(hr);
    //dd.Detach();

    ////// TEXTURE *
    //// Describe and create a Texture2D.
    //D3D12_RESOURCE_DESC textureDesc = {};
    //textureDesc.MipLevels = 1;
    //textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //textureDesc.Width = TextureWidth;
    //textureDesc.Height = TextureHeight;
    //textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    //textureDesc.DepthOrArraySize = 1;
    //textureDesc.SampleDesc.Count = 1;
    //textureDesc.SampleDesc.Quality = 0;
    //textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    //CD3DX12_HEAP_PROPERTIES txtHeapProps(D3D12_HEAP_TYPE_DEFAULT);

    //renderer->m_pDevice->CreateCommittedResource(
    //    &txtHeapProps,
    //    D3D12_HEAP_FLAG_NONE,
    //    &textureDesc,
    //    D3D12_RESOURCE_STATE_COPY_DEST,
    //    nullptr,
    //    IID_PPV_ARGS(&m_textureBuffer));

    //const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_textureBuffer, 0, 1);
    //// Create the GPU upload buffer.

    //CD3DX12_HEAP_PROPERTIES txtUploadProps(D3D12_HEAP_TYPE_UPLOAD);
    //CD3DX12_RESOURCE_DESC txtUploadDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    //renderer->m_pDevice->CreateCommittedResource(
    //    &txtUploadProps,
    //    D3D12_HEAP_FLAG_NONE,
    //    &txtUploadDesc,
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&m_uploadTexture));

    //// Copy data to the intermediate upload heap and then schedule a copy 
    //// from the upload heap to the Texture2D.
    //m_textureBitmap = GenerateTextureData();

    //D3D12_SUBRESOURCE_DATA textureData = {};
    //    textureData.pData = m_textureBitmap.data();
    //    textureData.RowPitch = TextureWidth * TexturePixelSize;
    //    textureData.SlicePitch = textureData.RowPitch * TextureHeight;

    //UpdateSubresources(renderer->m_pCommandList.Get(), m_textureBuffer, m_uploadTexture, 0, 0, 1, &textureData);


    //CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
    //    m_textureBuffer,
    //    D3D12_RESOURCE_STATE_COPY_DEST,
    //    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    //);
    //renderer->m_pCommandList->ResourceBarrier(1, &transitionBarrier);

    //// Describe and create a SRV for the texture.
    //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //srvDesc.Format = textureDesc.Format;
    //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //srvDesc.Texture2D.MipLevels = 1;
    //renderer->m_pDevice->CreateShaderResourceView(m_textureBuffer, &srvDesc, renderer->m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
}


