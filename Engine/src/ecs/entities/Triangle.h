
#include "../../include.h"
#include <d3d12.h>

#include "../components/Transform.h"

#include "GameObject.h"

class Renderer;
using namespace DirectX;

class Triangle : public GameObject {
public:
    Triangle();
    ~Triangle();
    void Initialize(Renderer* renderer);
    //void Update(float deltaTime, Renderer* renderer) override;
};
