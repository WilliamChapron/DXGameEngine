#pragma once

#include "../include.h"
using namespace DirectX;

struct Transform
{
    Transform();
    Transform(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scale);

    XMFLOAT3 position;
    XMFLOAT4 rotation;  // Utilisation d'un quaternion pour la rotation
    XMFLOAT3 scale;

    XMFLOAT4X4 matrix;

    XMFLOAT4X4 GetTransformMatrix() const;

    void UpdateTransformMatrix();
    void Rotate(float pitch, float roll, float yaw);
    void Translate(float offsetX, float offsetY, float offsetZ);
};
