#pragma once

#include <vector>
#include <DirectXMath.h>
#include "./include.h"
#include <d3d12.h>

class Renderer;

using namespace DirectX;

// Définition de la classe Vertex
struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

//struct ConstantBufferData {
//    XMFLOAT4X4 model;
//    XMFLOAT4X4 view;
//    XMFLOAT4X4 projection;
//};

// Définition de la classe Triangle
struct Triangle {
    XMFLOAT3 vertices[3]; // Coordonnées des sommets
    XMFLOAT4 color;       // Couleur du triangle
};


// Classe GameObject
class GameObject {
private:
    std::vector<Triangle> triangles; // Liste des triangles


    //ConstantBufferData m_cbData;

    //ID3D12Resource* m_constantBuffer;
    //UINT8* m_mappedConstantBuffer;

    // Vertex buffer
    ID3D12Resource* vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

    int renderCallnum = 0;

public:
    GameObject();
    ~GameObject();

    void Initialize(Renderer* renderer);

    void PopulateCommandList(Renderer* renderer);

    void WaitForPreviousFrame(Renderer* renderer);

    // Ajouter un triangle au gestionnaire
    void AddTriangle(const Triangle& triangle);

    // Supprimer un triangle du gestionnaire
    void RemoveTriangle(int index);

    // Mettre à jour un triangle existant
    void UpdateTriangle(int index, const Triangle& newTriangle);

    // Méthode de rendu des triangles
    void Render(Renderer* renderer);

};