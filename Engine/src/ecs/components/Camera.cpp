#include "Camera.h"

#include "../../include.h"
using namespace DirectX;

#include <iostream>




Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane){
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

    // Créer le quaternion de rotation à partir des angles d'Euler
    XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

    // Définir la direction de la caméra
    XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    // Faire pivoter les vecteurs de direction et de haut avec le quaternion de rotation
    forward = XMVector3Rotate(forward, rotationQuaternion);
    up = XMVector3Rotate(up, rotationQuaternion);
    right = XMVector3Rotate(right, rotationQuaternion);


    // Mettre à jour la cible de la caméra en fonction de la direction
    m_target.x = m_position.x + XMVectorGetX(forward);
    m_target.y = m_position.y + XMVectorGetY(forward);
    m_target.z = m_position.z + XMVectorGetZ(forward);

    // Mettre à jour le vecteur 'up' de la caméra
    m_up.x = XMVectorGetX(up);
    m_up.y = XMVectorGetY(up);
    m_up.z = XMVectorGetZ(up);

    // Mettre à jour la matrice de vue
    m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_target), XMLoadFloat3(&m_up));
    m_transposedViewMatrix = XMMatrixTranspose(m_viewMatrix);
    XMStoreFloat4x4(&f_viewMatrix, m_transposedViewMatrix);
}

//// Convertir les angles en radians
    //pitch = XMConvertToRadians(pitch);
    //yaw = XMConvertToRadians(yaw);
    //roll = XMConvertToRadians(roll);

    //XMVECTOR forwardVector = XMLoadFloat3(&vForward);
    //XMVECTOR rightVector = XMLoadFloat3(&vRight);
    //XMVECTOR upVector = XMLoadFloat3(&vUp);


    //XMVECTOR qRoll = XMQuaternionRotationAxis(forwardVector, roll);
    //XMVECTOR qPitch = XMQuaternionRotationAxis(rightVector, pitch);
    //XMVECTOR qYaw = XMQuaternionRotationAxis(upVector, yaw);
    //XMVECTOR q = XMQuaternionMultiply(qRoll, qPitch);
    //q = XMQuaternionMultiply(q, qYaw);


    ////Ajout rotation à quaternion / Multiplier qRotation stocké a q transform
    //XMVECTOR rotationQuaternion = XMLoadFloat4(&qRotation);
    //rotationQuaternion = XMQuaternionMultiply(rotationQuaternion, q);
    //XMStoreFloat4(&qRotation, rotationQuaternion);

    ////Convertir le quaternion en une matrice
    //XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuaternion);
    //XMStoreFloat4x4(&mRotation, rotationMatrix);

    //vRight.x = mRotation._11;
    //vRight.y = mRotation._12;
    //vRight.z = mRotation._13;

    //vUp.x = mRotation._21;
    //vUp.y = mRotation._22;
    //vUp.z = mRotation._23;

    //vForward.x = mRotation._31;
    //vForward.y = mRotation._32;
    //vForward.z = mRotation._33;
