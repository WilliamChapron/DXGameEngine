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

//void ColliderComponent::InitializeBoundingBox(GameObject* gameObject, Vertex* vertices, int numVertices) {
//    m_pGameObject = gameObject;
//
//	m_localAxisAlignedBoundingBox.min = { FLT_MAX, FLT_MAX, FLT_MAX };
//    m_localAxisAlignedBoundingBox.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
//
//
//    for (int i = 0; i < numVertices; ++i) {
//        XMFLOAT3 vertexPos = vertices[i].Pos;
//
//        m_localAxisAlignedBoundingBox.min.x = (m_localAxisAlignedBoundingBox.min.x < vertexPos.x) ? m_localAxisAlignedBoundingBox.min.x : vertexPos.x;
//        m_localAxisAlignedBoundingBox.min.y = (m_localAxisAlignedBoundingBox.min.y < vertexPos.y) ? m_localAxisAlignedBoundingBox.min.y : vertexPos.y;
//        m_localAxisAlignedBoundingBox.min.z = (m_localAxisAlignedBoundingBox.min.z < vertexPos.z) ? m_localAxisAlignedBoundingBox.min.z : vertexPos.z;
//
//        m_localAxisAlignedBoundingBox.max.x = (m_localAxisAlignedBoundingBox.max.x > vertexPos.x) ? m_localAxisAlignedBoundingBox.max.x : vertexPos.x;
//        m_localAxisAlignedBoundingBox.max.y = (m_localAxisAlignedBoundingBox.max.y > vertexPos.y) ? m_localAxisAlignedBoundingBox.max.y : vertexPos.y;
//        m_localAxisAlignedBoundingBox.max.z = (m_localAxisAlignedBoundingBox.max.z > vertexPos.z) ? m_localAxisAlignedBoundingBox.max.z : vertexPos.z;
//    }
//
//
//
//    //PRINT("Min");
//    //PRINT_VECTOR3(m_localAxisAlignedBoundingBox.min);
//    //PRINT("Max");
//    //PRINT_VECTOR3(m_localAxisAlignedBoundingBox.max);
//
//
//
//}

void ColliderComponent::InitializeBoundingBox(GameObject* gameObject, Vertex* vertices, int numVertices) {
    m_pGameObject = gameObject;

    m_localPoints.resize(numVertices);


    /*Transform* transformComponent = gameObject->GetComponent<Transform>(ComponentType::Transform);
    XMFLOAT4X4 transformMatrixFloat4x4 = transformComponent->GetTransformMatrix();
    XMMATRIX transformMatrix = XMLoadFloat4x4(&transformMatrixFloat4x4);

    for (int i = 0; i < numVertices; ++i) {
        XMFLOAT3 vertexPos = vertices[i].Pos;
        XMVECTOR localVertex = XMLoadFloat3(&vertexPos);
        XMVECTOR globalVertex = XMVector3TransformCoord(localVertex, transformMatrix);
        XMStoreFloat3(&m_localPoints[i], globalVertex);
    }*/


    //PRINT("Max");
    //PRINT_VECTOR3(m_localPoints[0]);


}


void ColliderComponent::Update(Renderer* renderer) {
    //PRINT("Update Collider");

}

// j'ai 8 points donc utilise les 8 points, passe les en global stocke les,, et au moment de l'update retrouve le min et le max des points global et compare les


//AABB ColliderComponent::TransformBoundingBoxLocalToGlobal(AABB localBoundingBox, GameObject* gameObject) 
//{
//    Transform* transformComponent = gameObject->GetComponent<Transform>(ComponentType::Transform);
//
//    XMVECTOR localMin = XMLoadFloat3(&localBoundingBox.min);
//    XMVECTOR localMax = XMLoadFloat3(&localBoundingBox.max);
//
//    XMFLOAT4X4 transformMatrixFloat4x4 = transformComponent->GetTransformMatrix();
//    XMMATRIX transformMatrix = XMLoadFloat4x4(&transformMatrixFloat4x4);
//
//    AABB globalBoundingBox;
//
//    XMVECTOR globalMin = XMVector3TransformCoord(localMin, transformMatrix);
//    XMVECTOR globalMax = XMVector3TransformCoord(localMax, transformMatrix);
//
//    XMStoreFloat3(&globalBoundingBox.min, globalMin);
//    XMStoreFloat3(&globalBoundingBox.max, globalMax);
//
//    return globalBoundingBox;
//}


bool ColliderComponent::CheckCollision(GameObject* collideObject) {

    //AABB box1 = m_localAxisAlignedBoundingBox;
    //AABB box2 = collideObject->GetComponent<ColliderComponent>(ComponentType::ColliderComponent)->m_localAxisAlignedBoundingBox;

    //AABB transformedBox1 = TransformBoundingBoxLocalToGlobal(box1, m_pGameObject);
    //AABB transformedBox2 = TransformBoundingBoxLocalToGlobal(box2, collideObject);

    ////std::cout << "Box 1 (Global):" << std::endl;
    ////std::cout << "  Min: (" << transformedBox1.min.x << ", " << transformedBox1.min.y << ", " << transformedBox1.min.z << ")" << std::endl;
    ////std::cout << "  Max: (" << transformedBox1.max.x << ", " << transformedBox1.max.y << ", " << transformedBox1.max.z << ")" << std::endl;

    ////std::cout << "Box 2 (Global):" << std::endl;
    ////std::cout << "  Min: (" << transformedBox2.min.x << ", " << transformedBox2.min.y << ", " << transformedBox2.min.z << ")" << std::endl;
    ////std::cout << "  Max: (" << transformedBox2.max.x << ", " << transformedBox2.max.y << ", " << transformedBox2.max.z << ")" << std::endl;


    //if (box1.max.x < box2.min.x || box1.min.x > box2.max.x) return false;
    //if (box1.max.y < box2.min.y || box1.min.y > box2.max.y) return false;
    //if (box1.max.z < box2.min.z || box1.min.z > box2.max.z) return false;

    return true;
}



