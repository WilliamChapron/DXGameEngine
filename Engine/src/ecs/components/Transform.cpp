#include "Transform.h"

Transform::Transform() : vPosition(0.0f, 0.0f, 0.0f), vRotation(0.0f, 0.0f, 0.0f, 1.0f), vScale(1.0f, 1.0f, 1.0f)
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

    XMMATRIX pitchMatrix = XMMatrixRotationX(pitch);
    XMMATRIX rollMatrix = XMMatrixRotationY(roll);
    XMMATRIX yawMatrix = XMMatrixRotationZ(yaw);

    XMMATRIX rotationMatrix = pitchMatrix * rollMatrix * yawMatrix;

    XMStoreFloat4x4(&mRotation, rotationMatrix);

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