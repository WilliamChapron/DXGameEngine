#include "Transform.h"

Transform::Transform() : vPosition(0.0f, 0.0f, 0.0f), qRotation(0.0f, 0.0f, 0.0f, 1.0f), vScale(1.0f, 1.0f, 1.0f)
{

    XMMATRIX positionMatrix = XMMatrixIdentity();
    XMStoreFloat4x4(&mPosition, positionMatrix);


    XMMATRIX rotationMatrix = XMMatrixIdentity();
    XMStoreFloat4x4(&mRotation, rotationMatrix);


    XMMATRIX scaleMatrix = XMMatrixIdentity();
    XMStoreFloat4x4(&mScale, scaleMatrix);


    XMMATRIX worldMatrix = XMMatrixIdentity();
    XMStoreFloat4x4(&mWorld, worldMatrix);


    vForward = XMFLOAT3(0.0f, 0.0f, 1.0f); 
    vRight = XMFLOAT3(1.0f, 0.0f, 0.0f);   
    vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);      
}

Transform::Transform(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scl) : vPosition(pos), vScale(scl)
{
    UpdateTransformMatrix();
}

XMFLOAT4X4 Transform::GetTransformMatrix() const
{
    return mWorld;
}

void Transform::UpdateTransformMatrix()
{
    XMMATRIX transformMatrix = XMLoadFloat4x4(&mScale) * XMLoadFloat4x4(&mRotation) * XMLoadFloat4x4(&mPosition);

    transformMatrix = XMMatrixTranspose(transformMatrix);

    XMStoreFloat4x4(&mWorld, transformMatrix);
}

void Transform::Rotate(float pitch, float roll, float yaw)
{
    XMVECTOR forwardVector = XMLoadFloat3(&vForward);
    XMVECTOR rightVector = XMLoadFloat3(&vRight);
    XMVECTOR upVector = XMLoadFloat3(&vUp);


    XMVECTOR qRoll = XMQuaternionRotationAxis(forwardVector, roll);
    XMVECTOR qPitch = XMQuaternionRotationAxis(rightVector, pitch);
    XMVECTOR qYaw = XMQuaternionRotationAxis(upVector, yaw);
    XMVECTOR q = XMQuaternionMultiply(qRoll, qPitch);
    q = XMQuaternionMultiply(q, qYaw);

    //Ajout rotation à quaternion / Multiplier qRotation stocké a q transform
    XMVECTOR rotationQuaternion = XMLoadFloat4(&qRotation);
    rotationQuaternion = XMQuaternionMultiply(rotationQuaternion, q);
    XMStoreFloat4(&qRotation, rotationQuaternion);

    //Convertir le quaternion en une matrice
    XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuaternion);
    XMStoreFloat4x4(&mRotation, rotationMatrix);

    //Mise à jour des axes
    vRight.x = mRotation._11;
    vRight.y = mRotation._12;
    vRight.z = mRotation._13;

    vUp.x = mRotation._21;
    vUp.y = mRotation._22;
    vUp.z = mRotation._23;

    vForward.x = mRotation._31;
    vForward.y = mRotation._32;
    vForward.z = mRotation._33;

    UpdateTransformMatrix();
}



void Transform::Translate(float offsetX, float offsetY, float offsetZ)
{

    XMMATRIX translationMatrix = XMMatrixTranslation(offsetX, offsetY, offsetZ);
    XMStoreFloat4x4(&mPosition, translationMatrix);

    UpdateTransformMatrix();
}

void Transform::Scale(float scaleX, float scaleY, float scaleZ) {
    XMMATRIX scalingMatrix = XMMatrixScaling(scaleX, scaleY, scaleZ);

    XMStoreFloat4x4(&mScale, scalingMatrix);
    UpdateTransformMatrix();
}