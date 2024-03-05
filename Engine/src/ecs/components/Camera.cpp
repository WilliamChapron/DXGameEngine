#include "Camera.h"

#include "../../include.h"
using namespace DirectX;

#include <iostream>




Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane) {
    // Init View Matrix
    m_position = XMFLOAT3(0.0f, 0.0f, -2.0f);
    m_target = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);

    m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_target), XMLoadFloat3(&m_up));
    m_projectionMatrix = XMMatrixTranspose(m_viewMatrix);
    XMStoreFloat4x4(&f_viewMatrix, m_transposedViewMatrix);

    m_projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);

    m_transposedProjectionMatrix = XMMatrixTranspose(m_projectionMatrix);
    XMStoreFloat4x4(&f_projectionMatrix, m_transposedProjectionMatrix);

    // D�clarer un quaternion pour stocker l'orientation actuelle de la cam�ra
    currentRotation = XMQuaternionIdentity();

}

void Camera::Update(float deltaTime) {
    // Calculer la matrice de vue
    m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_target), XMLoadFloat3(&m_up));
    m_projectionMatrix = XMMatrixTranspose(m_viewMatrix);
    XMStoreFloat4x4(&f_viewMatrix, m_projectionMatrix);
    std::cout << m_position.z << std::endl;
}

void Camera::UpdatePosition(XMFLOAT3 m_newPosition)
{
    m_position.x += m_newPosition.x;
    m_position.y += m_newPosition.y;
    m_position.z += m_newPosition.z;
}

void Camera::UpdatePosition(float x, float y, float z)
{

    m_position.x += x;
    m_position.y += y;
    m_position.z += z;
}

void Camera::UpdateTarget(XMFLOAT3 m_newTarget)
{
    m_position.x += m_newTarget.x;
    m_position.y += m_newTarget.y;
    m_position.z += m_newTarget.z;
}

XMFLOAT4X4 Camera::GetViewMatrix() const
{
    return f_viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
    return f_projectionMatrix;
}

void Camera::Rotate(float pitch, float yaw, float roll)
{

    // Convertir les angles en radians
    pitch = XMConvertToRadians(pitch);
    yaw = XMConvertToRadians(yaw);
    roll = XMConvertToRadians(roll);

    // Cr�er le quaternion de rotation � partir des angles d'Euler
    XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

    // Accumuler la rotation actuelle avec la nouvelle rotation
    currentRotation = XMQuaternionMultiply(currentRotation, rotationQuaternion);

    forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // Faire pivoter les vecteurs de direction et de haut avec le quaternion de rotation
    forward = XMVector3Rotate(forward, currentRotation);
    up = XMVector3Rotate(up, currentRotation);
    right = XMVector3Rotate(right, currentRotation);

    // Mettre � jour la cible de la cam�ra en fonction de la direction
    m_target.x = m_position.x + XMVectorGetX(forward);
    m_target.y = m_position.y + XMVectorGetY(forward);
    m_target.z = m_position.z + XMVectorGetZ(forward);

    // Mettre � jour le vecteur 'up' de la cam�ra
    m_up.x = XMVectorGetX(up);
    m_up.y = XMVectorGetY(up);
    m_up.z = XMVectorGetZ(up);
}




void Camera::RotateAroundTarget(float pitch, float yaw, float roll) {
    pitch = XMConvertToRadians(pitch);
    yaw = XMConvertToRadians(yaw);
    roll = XMConvertToRadians(roll);

    XMVECTOR positionRelativeToTarget = XMLoadFloat3(&m_position) - XMLoadFloat3(&m_target);
    XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
    positionRelativeToTarget = XMVector3Rotate(positionRelativeToTarget, rotationQuaternion);

    XMStoreFloat3(&m_position, positionRelativeToTarget + XMLoadFloat3(&m_target));
}