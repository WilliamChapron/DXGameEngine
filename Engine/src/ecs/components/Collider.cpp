#include "Mesh.h"

#include "Collider.h"

#include "../entities/GameObject.hpp"

#include "Component.h"

#include "Transform.h"

#include "../../core/Defines.h"

#include "Mesh.h"

#include <iostream>

#include "../../Utils.h"

using namespace DirectX;



ColliderComponent::ColliderComponent(std::string name) : Component(name, ComponentType::ColliderComponent) {

}


#define PRINT_VECTOR3(vec) std::cout << #vec << ": (" << (vec).x << ", " << (vec).y << ", " << (vec).z << ")" << std::endl;


void ColliderComponent::InitializeBoundingBox(GameObject* gameObject, Vertex* vertices, int numVertices) {
    m_pGameObject = gameObject;

    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = -FLT_MAX;
    float maxY = -FLT_MAX;
    float maxZ = -FLT_MAX;

    for (int i = 0; i < numVertices; ++i) {
        XMFLOAT3 vertexPos = vertices[i].Pos;

        minX = (minX < vertexPos.x) ? minX : vertexPos.x;
        minY = (minY < vertexPos.y) ? minY : vertexPos.y;
        minZ = (minZ < vertexPos.z) ? minZ : vertexPos.z;

        maxX = (maxX > vertexPos.x) ? maxX : vertexPos.x;
        maxY = (maxY > vertexPos.y) ? maxY : vertexPos.y;
        maxZ = (maxZ > vertexPos.z) ? maxZ : vertexPos.z;
    }

    m_size.x = abs(maxX - minX);
    m_size.y = abs(maxY - minY);
    m_size.z = abs(maxZ - minZ);
}


void ColliderComponent::Update(Renderer* renderer) {
    //PRINT("Update Collider");

}







bool ColliderComponent::CheckCollision(GameObject* collideObject) {
    XMFLOAT3 gPosBox1 = m_pGameObject->GetComponent<Transform>(ComponentType::Transform)->GetPosition();
    XMFLOAT3 gPosBox2 = collideObject->GetComponent<Transform>(ComponentType::Transform)->GetPosition();

    XMFLOAT3 sizeBox1 = m_size;
    XMFLOAT3 sizeBox2 = collideObject->GetComponent<ColliderComponent>(ComponentType::ColliderComponent)->m_size;

    float halfSizeX1 = sizeBox1.x * 0.5f;
    float halfSizeY1 = sizeBox1.y * 0.5f;
    float halfSizeZ1 = sizeBox1.z * 0.5f;

    float halfSizeX2 = sizeBox2.x * 0.5f;
    float halfSizeY2 = sizeBox2.y * 0.5f;
    float halfSizeZ2 = sizeBox2.z * 0.5f;

    XMFLOAT3 cornersBox1[8] = {
        { gPosBox1.x - halfSizeX1, gPosBox1.y - halfSizeY1, gPosBox1.z - halfSizeZ1 },
        { gPosBox1.x + halfSizeX1, gPosBox1.y - halfSizeY1, gPosBox1.z - halfSizeZ1 },
        { gPosBox1.x - halfSizeX1, gPosBox1.y + halfSizeY1, gPosBox1.z - halfSizeZ1 },
        { gPosBox1.x + halfSizeX1, gPosBox1.y + halfSizeY1, gPosBox1.z - halfSizeZ1 },
        { gPosBox1.x - halfSizeX1, gPosBox1.y - halfSizeY1, gPosBox1.z + halfSizeZ1 },
        { gPosBox1.x + halfSizeX1, gPosBox1.y - halfSizeY1, gPosBox1.z + halfSizeZ1 },
        { gPosBox1.x - halfSizeX1, gPosBox1.y + halfSizeY1, gPosBox1.z + halfSizeZ1 },
        { gPosBox1.x + halfSizeX1, gPosBox1.y + halfSizeY1, gPosBox1.z + halfSizeZ1 }
    };


    XMFLOAT3 cornersBox2[8] = {
        { gPosBox2.x - halfSizeX2, gPosBox2.y - halfSizeY2, gPosBox2.z - halfSizeZ2 },
        { gPosBox2.x + halfSizeX2, gPosBox2.y - halfSizeY2, gPosBox2.z - halfSizeZ2 },
        { gPosBox2.x - halfSizeX2, gPosBox2.y + halfSizeY2, gPosBox2.z - halfSizeZ2 },
        { gPosBox2.x + halfSizeX2, gPosBox2.y + halfSizeY2, gPosBox2.z - halfSizeZ2 },
        { gPosBox2.x - halfSizeX2, gPosBox2.y - halfSizeY2, gPosBox2.z + halfSizeZ2 },
        { gPosBox2.x + halfSizeX2, gPosBox2.y - halfSizeY2, gPosBox2.z + halfSizeZ2 },
        { gPosBox2.x - halfSizeX2, gPosBox2.y + halfSizeY2, gPosBox2.z + halfSizeZ2 },
        { gPosBox2.x + halfSizeX2, gPosBox2.y + halfSizeY2, gPosBox2.z + halfSizeZ2 }
    };

    // Imprimer les coins de la première boîte
    //std::cout << "Corners of Box 1:" << std::endl;
    //for (int i = 0; i < 8; ++i) {
    //    std::cout << "Corner " << i + 1 << ": ";
    //    printFloatWithPrecision(cornersBox1[i].x, 2);
    //    printFloatWithPrecision(cornersBox1[i].y, 2);
    //    printFloatWithPrecision(cornersBox1[i].z, 2);
    //    std::cout << std::endl;
    //}

    //// Imprimer les coins de la deuxième boîte
    //std::cout << "Corners of Box 2:" << std::endl;
    //for (int i = 0; i < 8; ++i) {
    //    std::cout << "Corner " << i + 1 << ": ";
    //    printFloatWithPrecision(cornersBox2[i].x, 2);
    //    printFloatWithPrecision(cornersBox2[i].y, 2);
    //    printFloatWithPrecision(cornersBox2[i].z, 2);
    //    std::cout << std::endl;
    //}

    XMFLOAT3 minBox1(FLT_MAX, FLT_MAX, FLT_MAX);
    XMFLOAT3 maxBox1(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; ++i) {
        minBox1.x = (cornersBox1[i].x < minBox1.x) ? cornersBox1[i].x : minBox1.x;
        minBox1.y = (cornersBox1[i].y < minBox1.y) ? cornersBox1[i].y : minBox1.y;
        minBox1.z = (cornersBox1[i].z < minBox1.z) ? cornersBox1[i].z : minBox1.z;

        maxBox1.x = (cornersBox1[i].x > maxBox1.x) ? cornersBox1[i].x : maxBox1.x;
        maxBox1.y = (cornersBox1[i].y > maxBox1.y) ? cornersBox1[i].y : maxBox1.y;
        maxBox1.z = (cornersBox1[i].z > maxBox1.z) ? cornersBox1[i].z : maxBox1.z;
    }

    XMFLOAT3 minBox2(FLT_MAX, FLT_MAX, FLT_MAX);
    XMFLOAT3 maxBox2(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; ++i) {
        minBox2.x = (cornersBox2[i].x < minBox2.x) ? cornersBox2[i].x : minBox2.x;
        minBox2.y = (cornersBox2[i].y < minBox2.y) ? cornersBox2[i].y : minBox2.y;
        minBox2.z = (cornersBox2[i].z < minBox2.z) ? cornersBox2[i].z : minBox2.z;

        maxBox2.x = (cornersBox2[i].x > maxBox2.x) ? cornersBox2[i].x : maxBox2.x;
        maxBox2.y = (cornersBox2[i].y > maxBox2.y) ? cornersBox2[i].y : maxBox2.y;
        maxBox2.z = (cornersBox2[i].z > maxBox2.z) ? cornersBox2[i].z : maxBox2.z;
    }



    //std::cout << "Box 1:" << std::endl;
    //std::cout << "  Min: ";
    //printFloatWithPrecision(minBox1.x, 2);
    //printFloatWithPrecision(minBox1.y, 2);
    //printFloatWithPrecision(minBox1.z, 2);
    //std::cout << "  Max: ";
    //printFloatWithPrecision(maxBox1.x, 2);
    //printFloatWithPrecision(maxBox1.y, 2);
    //printFloatWithPrecision(maxBox1.z, 2);

    //std::cout << "Box 2:" << std::endl;
    //std::cout << "  Min: ";
    //printFloatWithPrecision(minBox2.x, 2);
    //printFloatWithPrecision(minBox2.y, 2);
    //printFloatWithPrecision(minBox2.z, 2);
    //std::cout << "  Max: ";
    //printFloatWithPrecision(maxBox2.x, 2);
    //printFloatWithPrecision(maxBox2.y, 2);
    //printFloatWithPrecision(maxBox2.z, 2);



    bool collisionX = maxBox1.x >= minBox2.x && minBox1.x <= maxBox2.x;
    bool collisionY = maxBox1.y >= minBox2.y && minBox1.y <= maxBox2.y;
    bool collisionZ = maxBox1.z >= minBox2.z && minBox1.z <= maxBox2.z;

    return collisionX && collisionY && collisionZ;
}


