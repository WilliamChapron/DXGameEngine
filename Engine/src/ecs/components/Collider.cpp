#include "Mesh.h"

#include "Collider.h"

#include "../entities/GameObject.hpp"

#include "Component.h"

#include "Transform.h"

#include "../../core/Defines.h"

#include "Mesh.h"

#include <iostream>

using namespace DirectX;



ColliderComponent::ColliderComponent(std::string name) : Component(name, ComponentType::ColliderComponent) {

}


#define PRINT_VECTOR3(vec) std::cout << #vec << ": (" << (vec).x << ", " << (vec).y << ", " << (vec).z << ")" << std::endl;

void ColliderComponent::InitializeBoundingBox(GameObject* gameObject, Vertex* vertices, int numVertices) {
    m_pGameObject = gameObject;

	m_localAxisAlignedBoundingBox.min = { FLT_MAX, FLT_MAX, FLT_MAX };
    m_localAxisAlignedBoundingBox.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };





    for (int i = 0; i < numVertices; ++i) {
        XMFLOAT3 vertexPos = vertices[i].Pos;

        m_localAxisAlignedBoundingBox.min.x = (m_localAxisAlignedBoundingBox.min.x < vertexPos.x) ? m_localAxisAlignedBoundingBox.min.x : vertexPos.x;
        m_localAxisAlignedBoundingBox.min.y = (m_localAxisAlignedBoundingBox.min.y < vertexPos.y) ? m_localAxisAlignedBoundingBox.min.y : vertexPos.y;
        m_localAxisAlignedBoundingBox.min.z = (m_localAxisAlignedBoundingBox.min.z < vertexPos.z) ? m_localAxisAlignedBoundingBox.min.z : vertexPos.z;

        m_localAxisAlignedBoundingBox.max.x = (m_localAxisAlignedBoundingBox.max.x > vertexPos.x) ? m_localAxisAlignedBoundingBox.max.x : vertexPos.x;
        m_localAxisAlignedBoundingBox.max.y = (m_localAxisAlignedBoundingBox.max.y > vertexPos.y) ? m_localAxisAlignedBoundingBox.max.y : vertexPos.y;
        m_localAxisAlignedBoundingBox.max.z = (m_localAxisAlignedBoundingBox.max.z > vertexPos.z) ? m_localAxisAlignedBoundingBox.max.z : vertexPos.z;
    }

    //PRINT_VECTOR3(m_localAxisAlignedBoundingBox.min);
    //PRINT("Max");
    //PRINT_VECTOR3(m_localAxisAlignedBoundingBox.max);



}

void ColliderComponent::Update(Renderer* renderer) {
    //PRINT("Update Collider");

}


AABB ColliderComponent::TransformBoundingBoxLocalToGlobal(AABB& localBoundingBox) 
{
    Transform* transformComponent = m_pGameObject->GetComponent<Transform>(ComponentType::Transform);

    XMVECTOR localMin = XMLoadFloat3(&m_localAxisAlignedBoundingBox.min);
    XMVECTOR localMax = XMLoadFloat3(&m_localAxisAlignedBoundingBox.max);

    XMFLOAT4X4 transformMatrixFloat4x4 = transformComponent->GetTransformMatrix();
    XMMATRIX transformMatrix = XMLoadFloat4x4(&transformMatrixFloat4x4);

    AABB globalBoundingBox;

    XMVECTOR globalMin = XMVector3TransformCoord(localMin, transformMatrix);
    XMVECTOR globalMax = XMVector3TransformCoord(localMin, transformMatrix);



    return globalBoundingBox;

}


bool ColliderComponent::CheckCollision(GameObject* gameObject) {

    AABB box1 = TransformBoundingBoxLocalToGlobal(m_localAxisAlignedBoundingBox);

    AABB box2 = TransformBoundingBoxLocalToGlobal(gameObject->GetComponent<ColliderComponent>(ComponentType::ColliderComponent)->m_localAxisAlignedBoundingBox);


    if (box1.max.x < box2.min.x || box1.min.x > box2.max.x) return false;
    if (box1.max.y < box2.min.y || box1.min.y > box2.max.y) return false;
    if (box1.max.z < box2.min.z || box1.min.z > box2.max.z) return false;

    return true;
}



