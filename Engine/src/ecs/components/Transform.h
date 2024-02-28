#pragma once

#include "../../include.h"
using namespace DirectX;

struct Transform
{
    Transform();
    Transform(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scale);


    // Translate data
    XMFLOAT3 vPosition;
    XMFLOAT4X4 mPosition;

    // Rotate data
    XMFLOAT4 qRotation;  // Quaternion 
    XMFLOAT4X4 mRotation;


    // Scale data
    XMFLOAT3 vScale;
    XMFLOAT4X4 mScale;


    // RIGHT HANDED ?
    // Direction vector 
    XMFLOAT3 vForward;
    XMFLOAT3 vRight;
    XMFLOAT3 vUp;


    // World Matrix
    XMFLOAT4X4 mWorld;

    // Getter
    XMFLOAT4X4 GetTransformMatrix() const;

    // Update them between each other
    void UpdateTransformMatrix();

    // Transform
    void Translate(float offsetX, float offsetY, float offsetZ);
    void Rotate(float pitch, float roll, float yaw);
    void Scale(float scaleX, float scaleY, float scaleZ);

};
