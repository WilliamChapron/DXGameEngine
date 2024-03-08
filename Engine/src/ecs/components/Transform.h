#pragma once

#include "../../include.h"
#include "./Component.h"

using namespace DirectX;


class Transform : public Component
{
public:
    Transform(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scale);
    //Transform();

    void Init();
    void Update();

    // Getter
    XMFLOAT4X4 GetTransformMatrix() const;
    XMFLOAT3 GetPosition() const;
    XMFLOAT3 GetScale() const;
    XMFLOAT3 GetRotation() const;

    inline XMFLOAT4X4 GetRotationMatrix() {
        return mRotation;
    }

    inline XMFLOAT4 GetRotationQuaternion() {
        return qRotation;
    }

    //XMFLOAT3 QuaternionToEulerAngles(const XMFLOAT4& quaternion);

    // Update them between each other
    void UpdateTransformMatrix();

    // Transform
    void SetPosition(float offsetX, float offsetY, float offsetZ);
    void Translate(float offsetX, float offsetY, float offsetZ);
    void Rotate(float pitch, float roll, float yaw);
    void SetScale(float scaleX, float scaleY, float scaleZ);
    void SetRotation(float pitch, float roll, float yaw);

private:
    // Translate data
    XMFLOAT3 vPosition;
    XMFLOAT4X4 mPosition;

    // Rotate data
    XMFLOAT4 qRotation;  // Quaternion 
    XMFLOAT3 vRotation;  // Vector
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
};