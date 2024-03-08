//#include "Mesh.h"
//#include "ColliderComponent.h"
//#include "../entities/GameObject.hpp"
//#include "Transform.h"
//#include <iostream>
//
//void ColliderComponent::Update(Renderer* renderer) {
//
//}
//
//
//ColliderComponent::ColliderComponent(std::string name) : Component(name, ComponentType::Collider) {
//
//}
//
////void ColliderComponent::InitializeBoundingBox(GameObject* gameObject) {
////
////    m_baseBoundingBox = 
////
////
////}
//
//void ColliderComponent::UpdateBoundingBox(GameObject* gameObject) {
//
//
//    m_axisAlignedBoundingBox.min = { FLT_MAX, FLT_MAX, FLT_MAX };
//    m_axisAlignedBoundingBox.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
//
//
//
//    for (int i = 0; i < 8; ++i) {
//        XMFLOAT3 localPosition = gameObject->GetComponent<Mesh>(ComponentType::Transform).;
//        XMFLOAT3& globalPosition = ;
//
//        m_axisAlignedBoundingBox.min.x = std::min(m_axisAlignedBoundingBox.min.x, adjustedGlobalPosition.x);
//        m_axisAlignedBoundingBox.min.y = std::min(m_axisAlignedBoundingBox.min.y, adjustedGlobalPosition.y);
//        m_axisAlignedBoundingBox.min.z = std::min(m_axisAlignedBoundingBox.min.z, adjustedGlobalPosition.z);
//
//        m_axisAlignedBoundingBox.max.x = std::max(m_axisAlignedBoundingBox.max.x, adjustedGlobalPosition.x);
//        m_axisAlignedBoundingBox.max.y = std::max(m_axisAlignedBoundingBox.max.y, adjustedGlobalPosition.y);
//        m_axisAlignedBoundingBox.max.z = std::max(m_axisAlignedBoundingBox.max.z, adjustedGlobalPosition.z);
//    }
//}
//
//bool ColliderComponent::CheckCollision(const AABB& box2) {
//    if (m_axisAlignedBoundingBox.max.x < box2.min.x || m_axisAlignedBoundingBox.min.x > box2.max.x) return false;
//    if (m_axisAlignedBoundingBox.max.y < box2.min.y || m_axisAlignedBoundingBox.min.y > box2.max.y) return false;
//    if (m_axisAlignedBoundingBox.max.z < box2.min.z || m_axisAlignedBoundingBox.min.z > box2.max.z) return false;
//
//    return true;
//}
//
//
//
