#pragma once

#include "../../include.h"
using namespace DirectX;

class Camera
{
public:
    Camera();
    ~Camera();

    XMFLOAT4X4 GetViewMatrix() const;
    XMFLOAT4X4 GetProjectionMatrix() const;

private:
    float m_fieldOfView;
    float m_aspectRatio;

    XMFLOAT4X4 m_viewMatrix;
    XMFLOAT4X4 m_projectionMatrix;
};
