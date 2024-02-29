
#include "../../include.h"

#include "../components/Transform.h"

#include "GameObject.h"

class Renderer;
class Camera;
using namespace DirectX;

class Triangle : public GameObject {
public:
    Triangle();
    ~Triangle();
    void Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);
    //void Update(float deltaTime, Renderer* renderer) override;
};
