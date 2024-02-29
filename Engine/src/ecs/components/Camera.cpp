#include "Camera.h"

#include "../../include.h"
using namespace DirectX;




Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane){
    // Init View Matrix
    m_position = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
    m_target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    m_viewMatrix = XMMatrixLookAtLH(m_position, m_target, m_up);
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::Update(float deltaTime) {
    // Calculer la matrice de vue
    m_viewMatrix = XMMatrixLookAtLH(m_position, m_target, m_up);
}

void Camera::UpdatePosition(XMVECTOR m_newPosition)
{
    m_position = m_newPosition;
}

void Camera::UpdateTarget(XMVECTOR m_newTarget)
{
    m_target = m_newTarget;
}

XMFLOAT4X4 Camera::GetViewMatrix() const
{
    XMFLOAT4X4 matrix;
    XMStoreFloat4x4(&matrix, m_viewMatrix);
    return matrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
    XMFLOAT4X4 matrix;
    XMStoreFloat4x4(&matrix, m_projectionMatrix);
    return matrix;
}