#pragma once




//#include "../../include.h"
#include <DirectXMath.h>
#include "GameObject.h"

class Renderer;
class Camera;
using namespace DirectX;

class CubeMesh : public GameObject {
public:
    CubeMesh();
    ~CubeMesh();
    void Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);
    //void Update(float deltaTime, Renderer* renderer) override;
};
