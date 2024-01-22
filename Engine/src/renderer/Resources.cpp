#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

struct Vertex {
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
};

struct ObjectConstants {
    XMFLOAT4X4 World;
    XMFLOAT4X4 View;
    XMFLOAT4X4 Proj;
};

struct Mesh {
    ComPtr<ID3D12Resource> VertexBuffer;
    ComPtr<ID3D12Resource> IndexBuffer;
    UINT VertexCount;
    UINT IndexCount;
};

struct Material {
    XMFLOAT4 AmbientColor;
    XMFLOAT4 DiffuseColor;
    XMFLOAT4 SpecularColor;
};

struct GameObject {
    Mesh MeshData;
    Material ObjectMaterial;
    ObjectConstants ObjectConstantsData;
};

class ResourceManager {
public:
    ResourceManager(ID3D12Device* device);
    ~ResourceManager();

    Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);
    Material CreateMaterial(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular);
    GameObject CreateGameObject(const Mesh& mesh, const Material& material);

private:
    ID3D12Device* m_Device;
    std::vector<ComPtr<ID3D12Resource>> m_UploadBuffers; // Utilisé pour stocker les données temporaires
};
