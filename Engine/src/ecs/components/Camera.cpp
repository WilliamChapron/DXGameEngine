#include "Camera.h"

#include "../../include.h"
using namespace DirectX;

#include <iostream>




Camera::Camera(float _fov, float _aspectRatio, float _nearPlane, float _farPlane) : fov(_fov), aspectRatio(_aspectRatio), nearPlane(_nearPlane), farPlane(_farPlane){
    // Init View Matrix
    m_position = XMFLOAT3(0.0f, -5.0f, -2.0f);
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
}

void Camera::UpdatePosition(XMFLOAT3 displacement)
{
    XMVECTOR m_displacement = XMVector3Rotate(XMLoadFloat3(&displacement), currentRotation);

    // Mettre à jour la position de la caméra en tenant compte du vecteur de déplacement
    XMVECTOR newPosition = XMLoadFloat3(&m_position) + m_displacement;
    XMStoreFloat3(&m_position, newPosition);

    // Mettre à jour la cible de la caméra en fonction de la nouvelle position
    m_target.x = m_position.x + XMVectorGetX(forward);
    m_target.y = m_position.y + XMVectorGetY(forward);
    m_target.z = m_position.z + XMVectorGetZ(forward);
}

void Camera::UpdatePosition(float x, float y, float z)
{
    // Calculer le vecteur de translation
    // Transformer le vecteur de déplacement par la rotation actuelle
    XMFLOAT3 m_displacementVector(x, y, z);
    XMVECTOR m_displacement = XMVector3Rotate(XMLoadFloat3(&m_displacementVector), currentRotation);

    // Mettre à jour la position de la caméra en tenant compte du vecteur de déplacement
    XMVECTOR newPosition = XMLoadFloat3(&m_position) + m_displacement;
    XMStoreFloat3(&m_position, newPosition);

    // Mettre à jour la cible de la caméra en fonction de la nouvelle position
    m_target.x = m_position.x + XMVectorGetX(forward);
    m_target.y = m_position.y + XMVectorGetY(forward);
    m_target.z = m_position.z + XMVectorGetZ(forward);

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

    // Appliquer la rotation actuelle au quaternion de rotation
    rotationQuaternion = XMQuaternionMultiply(currentRotation, rotationQuaternion);

    // Mettre à jour la rotation actuelle
    currentRotation = rotationQuaternion;

    // Mettre à jour la direction de la caméra en fonction de la nouvelle rotation
    XMVECTOR forwardVector = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationQuaternion);

    // Calculer le vecteur de droite (rightVector) en croisant forwardVector avec le vecteur up de la caméra
    XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR rightVector = XMVector3Cross(forwardVector, upVector);
    rightVector = XMVector3Normalize(rightVector);

    // Mettre à jour la direction de droite de la caméra en fonction de la nouvelle rotation
    right = XMVector3Rotate(rightVector, rotationQuaternion);

    // Calculer le vecteur vers le haut (upVector) en croisant forwardVector avec rightVector
    upVector = XMVector3Cross(rightVector, forwardVector);
    upVector = XMVector3Normalize(upVector);

    // Mettre à jour la direction vers le haut de la caméra en fonction de la nouvelle rotation
    up = XMVector3Rotate(upVector, rotationQuaternion);

    // Mettre à jour m_target en fonction de la nouvelle direction avant
    XMFLOAT3 fForwardVector;
    XMStoreFloat3(&fForwardVector ,forwardVector);
    m_target.x = m_position.x + fForwardVector.x;
    m_target.y = m_position.y + fForwardVector.y;
    m_target.z = m_position.z + fForwardVector.z;




    //// Convertir les angles en radians
    //pitch = XMConvertToRadians(pitch);
    //yaw = XMConvertToRadians(yaw);
    //roll = XMConvertToRadians(roll);

    //// Créer le quaternion de rotation à partir des angles d'Euler
    //XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

    //// Appliquer la rotation actuelle au quaternion de rotation
    //rotationQuaternion = XMQuaternionMultiply(currentRotation, rotationQuaternion);

    //// Mettre à jour la rotation actuelle
    //currentRotation = rotationQuaternion;
    //XMVECTOR forwardVector = XMVector3Normalize(XMLoadFloat3(&m_target) - XMLoadFloat3(&m_position));

    //// Mettre à jour la direction de la caméra en fonction de la nouvelle rotation
    //forward = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationQuaternion);
    //
    //// Calculer le vecteur de droite (rightVector) en croisant forwardVector avec le vecteur up de la caméra
    //XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    //XMVECTOR rightVector = XMVector3Cross(forwardVector, upVector);
    //rightVector = XMVector3Normalize(rightVector);

    //// Mettre à jour la direction de droite de la caméra en fonction de la nouvelle rotation
    //right = XMVector3Rotate(rightVector, rotationQuaternion);

    //// Calculer le vecteur vers le haut (upVector) en croisant forwardVector avec rightVector
    //upVector = XMVector3Cross(rightVector, forwardVector);
    //upVector = XMVector3Normalize(upVector);

    //// Mettre à jour la direction vers le haut de la caméra en fonction de la nouvelle rotation
    //up = XMVector3Rotate(upVector, rotationQuaternion);


    //m_target.x = m_position.x + XMVectorGetX(forward);
    //m_target.y = m_position.y + XMVectorGetY(forward);
    //m_target.z = m_position.z + XMVectorGetZ(forward);

    //// Convertir les angles en radians
    //pitch = XMConvertToRadians(pitch);
    //yaw = XMConvertToRadians(yaw);
    //roll = XMConvertToRadians(roll);

    //// Cr�er le quaternion de rotation � partir des angles d'Euler
    //XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);


    //// Accumuler la rotation actuelle avec la nouvelle rotation
    //currentRotation = XMQuaternionMultiply(currentRotation, rotationQuaternion);

    //forward = XMVectorSet(0.0f, 0.0f, m_position.z, 0.0f);
    //right = XMVectorSet(m_position.x, 0.0f, 0.0f, 0.0f);
    //up = XMVectorSet(0.0f, m_position.y, 0.0f, 0.0f);

    //// Faire pivoter les vecteurs de direction et de haut avec le quaternion de rotation

    //forward = XMVector3Rotate(forward, currentRotation);
    //up = XMVector3Rotate(up, currentRotation);
    //right = XMVector3Rotate(right, currentRotation);



    //// Mettre � jour la cible de la cam�ra en fonction de la direction

    //m_target.x = m_position.x + XMVectorGetX(forward);
    //m_target.y = m_position.y + XMVectorGetY(forward);
    //m_target.z = m_position.z + XMVectorGetZ(forward);


    //// Convertir les angles en radians
    //pitch = XMConvertToRadians(pitch);
    //yaw = XMConvertToRadians(yaw);
    //roll = XMConvertToRadians(roll);

    //// Créer le quaternion de rotation à partir des angles d'Euler
    //XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

    //// Appliquer la rotation actuelle au quaternion de rotation
    //rotationQuaternion = XMQuaternionMultiply(currentRotation, rotationQuaternion);

    //// Mettre à jour la rotation actuelle
    //currentRotation = rotationQuaternion;

    //// Calculer le vecteur entre la position de la caméra et la cible
    //XMVECTOR cameraToTarget = XMVectorSubtract(XMLoadFloat3(&m_target), XMLoadFloat3(&m_position));

    //// Appliquer la rotation au vecteur entre la caméra et la cible
    //cameraToTarget = XMVector3Rotate(cameraToTarget, rotationQuaternion);

    //// Mettre à jour la position de la caméra en fonction de la nouvelle position de la cible
    //XMVECTOR newPosition = XMVectorSubtract(XMLoadFloat3(&m_target), cameraToTarget);
    //XMStoreFloat3(&m_position, newPosition);

    //// Mettre à jour les vecteurs de direction et de haut en fonction de la nouvelle position de la caméra et de la cible
    //forward = XMVector3Normalize(cameraToTarget);
    //XMVECTOR worldUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    //right = XMVector3Normalize(XMVector3Cross(worldUp, forward));
    //up = XMVector3Cross(forward, right);

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