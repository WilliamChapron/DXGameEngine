#pragma once

#include "../../include.h"
using namespace DirectX;

class Camera
{
public:
    Camera(float fov = XM_PIDIV4, float aspectRatio = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
    ~Camera() {};

    void Update(float deltaTime);
    void UpdatePosition(XMVECTOR m_newPosition);
    void UpdateTarget(XMVECTOR m_newTarget);

    XMFLOAT4X4 GetViewMatrix() const;
    XMFLOAT4X4 GetProjectionMatrix() const;

private:
    DirectX::XMVECTOR m_position;
    DirectX::XMVECTOR m_target;
    DirectX::XMVECTOR m_up;

    // j'ai utilisé XMMATRIX parce que XMMatrixPerspectiveFovLH 
    // attends un XMMATRIX et pas un XMFLOAT4X4
    XMMATRIX m_viewMatrix;
    XMMATRIX m_projectionMatrix;
};
