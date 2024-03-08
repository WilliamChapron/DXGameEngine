//#pragma once
//
//#include "../../include.h"
//
//
//#include "../components/Component.h"
//
//using namespace DirectX;
//
//struct AABB {
//    XMFLOAT3 min;
//    XMFLOAT3 max;
//};
//
//struct Vertex;
//class GameObject;
//
//class ColliderComponent : public Component
//{
//public:
//    ColliderComponent(std::string name);
//    void Update(Renderer* renderer) override;
//
//    void OnEnter();
//    void OnStay();
//    void OnExit();
//
//
//    void InitializeBoundingBox(GameObject* gameObject);
//    void UpdateBoundingBox(GameObject* gameObject);
//
//    bool CheckCollision(const AABB & box2);
//      
//
//private:
//
//    AABB m_axisAlignedBoundingBox;
//    AABB m_baseBoundingBox
//
//};
