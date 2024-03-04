#include "Camera.h"


Camera::Camera() {
    // Init View Matrix
    XMFLOAT3 eye(0.0f, 0.0f, -2.0f);    // Position de la caméra
    XMFLOAT3 at(0.0f, 0.0f, 0.0f);      // Point où la caméra regarde
    XMFLOAT3 up(0.0f, 1.0f, 0.0f);      // Vecteur "up" (orienté vers le haut)
    XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up));
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
    XMStoreFloat4x4(&m_viewMatrix, transposedViewMatrix);

    // Init Project Matrix
    float aspectRatio = 16.0f / 9.0f;  
    float fieldOfView = XM_PIDIV4;  // 45 degrés
    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, 0.1f, 100.0f);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMStoreFloat4x4(&m_projectionMatrix, transposedProjectionMatrix);
}

Camera::~Camera()

{
}

XMFLOAT4X4 Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}
