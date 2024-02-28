
#include "./include.h"
#include <d3d12.h>

#include "./physics/Transform.h"

//struct Transform;

class Renderer;
using namespace DirectX;
#include "./GameObject.h"

class Triangle : public GameObject {
public:
    Triangle();
    ~Triangle();
    void Initialize(Renderer* renderer);


private:


};
