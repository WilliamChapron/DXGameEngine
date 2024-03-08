#pragma once

#include "../../include.h"


#include "../components/Component.h"

#include <vector>

using namespace DirectX;

struct AABB {
    XMFLOAT3 min;
    XMFLOAT3 max;
};

struct Vertex;
class GameObject;
struct Vertex;

class ColliderComponent : public Component
{
public:
    ColliderComponent(std::string name);
    void Update(Renderer* renderer) override;

    void OnEnter();
    void OnStay();
    void OnExit();


    void InitializeBoundingBox(GameObject* gameObject, Vertex* vertices, int numVertices);
    //AABB TransformBoundingBoxLocalToGlobal(AABB localBoundingBox, GameObject* gameObject);

    bool CheckCollision(GameObject* gameObject);
      

private:
    GameObject* m_pGameObject;
    AABB m_localAxisAlignedBoundingBox;
    XMFLOAT3 m_halfSize;
};
