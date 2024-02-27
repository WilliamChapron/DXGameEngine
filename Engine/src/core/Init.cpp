#include "Init.h"
#include <DirectXMath.h>

void Init::InitializeMatrices(DirectX::XMFLOAT4X4& model, DirectX::XMFLOAT4X4& view, DirectX::XMFLOAT4X4& projection) {
    // Initialisation de la matrice de modèle (model)
    DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX transposedModelMatrix = DirectX::XMMatrixTranspose(modelMatrix);
    DirectX::XMStoreFloat4x4(&model, transposedModelMatrix);

    // Initialisation de la matrice de vue (view)
    DirectX::XMFLOAT3 eye(0.0f, 0.0f, -2.0f);    // Position de la caméra
    DirectX::XMFLOAT3 at(0.0f, 0.0f, 0.0f);      // Point où la caméra regarde
    DirectX::XMFLOAT3 up(0.0f, 1.0f, 0.0f);      // Vecteur "up" (orienté vers le haut)
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&at), DirectX::XMLoadFloat3(&up));
    DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
    DirectX::XMStoreFloat4x4(&view, transposedViewMatrix);

    // Initialisation de la matrice de projection
    float aspectRatio = 16.0f / 9.0f;   // Vous devrez fournir la valeur de l'aspect ratio
    float fieldOfView = DirectX::XM_PIDIV4;  // Angle de champ de vision (45 degrés ici)
    DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, 0.1f, 100.0f);
    DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
    DirectX::XMStoreFloat4x4(&projection, transposedProjectionMatrix);
}

//
