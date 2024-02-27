#include "Transform.h"

Transform::Transform() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0f, 1.0f, 1.0f)
{
    XMMATRIX modelMatrix = XMMatrixIdentity();  
    XMMATRIX transposedModelMatrix = XMMatrixTranspose(modelMatrix);
    XMStoreFloat4x4(&matrix, transposedModelMatrix);
}

Transform::Transform(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scl)
    : position(pos), scale(scl)
{


    // Init transform Matrix
    UpdateTransformMatrix();
}

XMFLOAT4X4 Transform::GetTransformMatrix() const
{
    return matrix;
}

void Transform::UpdateTransformMatrix()
{

    XMMATRIX transformMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&scale)) * XMMatrixRotationQuaternion(XMLoadFloat4(&rotation)) * XMMatrixTranslationFromVector(XMLoadFloat3(&position));

    transformMatrix = XMMatrixTranspose(transformMatrix);
    XMStoreFloat4x4(&matrix, transformMatrix);
}

void Transform::Rotate(float pitch, float roll, float yaw)
{
    float pitchRadians = XMConvertToRadians(pitch);
    float rollRadians = XMConvertToRadians(roll);
    float yawRadians = XMConvertToRadians(yaw);

    // Création des quaternions de rotation
    XMVECTOR pitchQuaternion = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), pitchRadians);
    XMVECTOR rollQuaternion = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rollRadians);
    XMVECTOR yawQuaternion = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), yawRadians);

    // Multiplication des quaternions pour obtenir la rotation combinée
    XMVECTOR combinedRotation = XMQuaternionMultiply(XMQuaternionMultiply(pitchQuaternion, rollQuaternion), yawQuaternion);

    // Convertir le quaternion résultant en une matrice de rotation
    XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(combinedRotation);

    // Appliquer la rotation à la matrice actuelle
    XMMATRIX currentMatrix = XMLoadFloat4x4(&matrix);
    currentMatrix = XMMatrixMultiply(currentMatrix, rotationMatrix);

    currentMatrix = XMMatrixTranspose(currentMatrix);
    XMStoreFloat4x4(&matrix, currentMatrix); 
}

void Transform::Translate(float offsetX, float offsetY, float offsetZ)
{
    XMMATRIX translationMatrix = XMMatrixTranslation(offsetX, offsetY, offsetZ);
    XMMATRIX currentMatrix = XMLoadFloat4x4(&matrix);


    currentMatrix = XMMatrixMultiply(currentMatrix, translationMatrix);
    currentMatrix = XMMatrixTranspose(currentMatrix);
    XMStoreFloat4x4(&matrix, currentMatrix);
}