#pragma once

#include "../../include.h"
using namespace DirectX;

class Camera
{
public:
    Camera(float fov = XM_PIDIV4, float aspectRatio = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
    ~Camera() {};

    void Update(float deltaTime);
    void UpdatePosition(XMFLOAT3 m_newPosition);
    void UpdatePosition(float x, float y, float z);


    void UpdateTarget(XMFLOAT3 m_newTarget);

    XMFLOAT4X4 GetViewMatrix() const;
    XMFLOAT4X4 GetProjectionMatrix() const;

    void Rotate(float pitch, float yaw, float roll);
    void RotateAroundTarget(float pitch, float yaw, float roll);


private:
    XMVECTOR currentRotation;

    // Rotate data
    XMFLOAT3 qRotation;  // Quaternion 
    XMFLOAT4X4 mRotation;

    // Direction vector 
    XMVECTOR forward;
    XMVECTOR up;
    XMVECTOR right;

    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT3 m_target;
    DirectX::XMFLOAT3 m_up;

    XMMATRIX m_viewMatrix;
    XMMATRIX m_projectionMatrix;
    XMMATRIX m_transposedViewMatrix;
    XMMATRIX m_transposedProjectionMatrix;

    XMFLOAT4X4 f_viewMatrix;
    XMFLOAT4X4 f_projectionMatrix;
};
