#include "GameObjectManager.h"
#include "../../renderer/Graphics.h" // renderer
#include "../components/Component.h" // renderer
#include "../components/Collider.h" // renderer

#include "../entities/GameObject.hpp"



GameObjectManager::GameObjectManager(Camera* camera) : m_pCamera(camera) {}

// Ajouter un objet au gestionnaire
void GameObjectManager::AddObject(const std::string& name, GameObject* object) {
    objectMap[name] = object;
    PRINT("Object added: " << name);
}

// Supprimer un objet du gestionnaire
void GameObjectManager::RemoveObject(const std::string& name) {
    auto it = objectMap.find(name);
    if (it != objectMap.end()) {
        objectMap.erase(it);
        PRINT("Object erased: " << name);
    }


}

//#define PRINT_VECTOR3(vec) std::cout << #vec << ": (" << XMVectorGetX(vec) << ", " << XMVectorGetY(vec) << ", " << XMVectorGetZ(vec) << ")" << std::endl;
//
//bool IsColliding(XMFLOAT3& vectorPositionInput, XMFLOAT3& vectorPositionOutput) {
//
//
//    XMVECTOR inputVector = XMLoadFloat3(&vectorPositionInput);
//    XMVECTOR outputVector = XMLoadFloat3(&vectorPositionOutput);
//
//    float length = XMVector3Length(inputVector - outputVector).m128_f32[0];
//
//    XMFLOAT3 normalizedDistance;
//    normalizedDistance.x = (vectorPositionInput.x - vectorPositionOutput.x);
//    normalizedDistance.y = (vectorPositionInput.y - vectorPositionOutput.y);
//    normalizedDistance.z = (vectorPositionInput.z - vectorPositionOutput.z);
//
//    //PRINT(normalizedDistance.x);
//
//
//    normalizedDistance.x = abs(normalizedDistance.x);
//    normalizedDistance.y = abs(normalizedDistance.y);
//    normalizedDistance.z = abs(normalizedDistance.z);
//
//    PRINT("Normalize");
//    PRINT(normalizedDistance.x);
//    PRINT(normalizedDistance.y);
//    PRINT(normalizedDistance.z);
//    PRINT("Normalize end");
//
//    if (normalizedDistance.x == 0 && normalizedDistance.y == 0 && normalizedDistance.x == 0) {
//        PRINT("EEEEEEEEEEEEEEEEEEEEEEEEEEE");
//        PRINT("EEEEEEEEEEEEEEEEEEEEEEEEEEE");
//        PRINT("EEEEEEEEEEEEEEEEEEEEEEEEEEE");
//        PRINT("EEEEEEEEEEEEEEEEEEEEEEEEEEE");
//        PRINT("EEEEEEEEEEEEEEEEEEEEEEEEEEE");
//    }
//
//
//    return normalizedDistance.x <= 0 && normalizedDistance.y <= 0 && normalizedDistance.x <= 0;
//}
//
struct TestedPair {
    std::string first;
    std::string second;

    bool operator == (const TestedPair& other) const {
        return (first == other.second);
    }
};


void GameObjectManager::Update(Renderer* renderer) {
    HRESULT hr;

    //PRINT("Rendering...");

    renderer->Precommandlist();



    PRINT("Frame");
    std::vector<TestedPair> testedPairs;

    for (auto& pair : objectMap) {
        GameObject* gameObject = pair.second;

        gameObject->Update(renderer, m_pCamera);

        // tryCollide = object on which we test collision
        for (auto& tryCollide : objectMap) {
            if (tryCollide.first == pair.first) {
                continue;
            }

            TestedPair objectPair{ pair.first, tryCollide.first };

            auto it = std::find(testedPairs.begin(), testedPairs.end(), objectPair);
            if (it != testedPairs.end()) {
                continue;
            }

            ColliderComponent* colliderComponent = gameObject->GetComponent<ColliderComponent>(ComponentType::ColliderComponent);


            std::cout << "Check Collide Object : " << pair.first << " With : " << tryCollide.first << std::endl;
            PRINT(colliderComponent->CheckCollision(tryCollide.second));

            testedPairs.push_back(objectPair);
        }
    }

    //for (const auto& pair : testedPairs) {
    //    std::cout << "Tested Pair: (" << pair.first << ", " << pair.second << ")\n";
    //}

    // CHECK COLLIDING


    renderer->Postcommandlist();

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    hr = renderer->m_pSwapChain->Present(1, 0);
    renderer->m_frameIndex = (renderer->m_frameIndex + 1) % renderer->m_FRAME_COUNT;
    ASSERT_FAILED(hr);

    renderer->WaitForPreviousFrame();

    //PRINT("Rendering complete");
};