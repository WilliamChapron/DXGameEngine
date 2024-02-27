#pragma once

#include <DirectXMath.h>

class Init {
public:
    static void InitializeMatrices(DirectX::XMFLOAT4X4& model, DirectX::XMFLOAT4X4& view, DirectX::XMFLOAT4X4& projection);
};
