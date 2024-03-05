#include "GameObjectManager.h"
#include "../../renderer/Graphics.h" // renderer
#include "../components/Component.h" // renderer

// Constructeur par d�faut
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

#define PRINT_VECTOR3(vec) std::cout << #vec << ": (" << XMVectorGetX(vec) << ", " << XMVectorGetY(vec) << ", " << XMVectorGetZ(vec) << ")" << std::endl;

bool IsColliding(XMFLOAT3& vectorPositionInput, XMFLOAT3& vectorPositionOutput) {
    // Convertir XMFLOAT3 en XMVECTOR
    XMVECTOR inputVector = XMLoadFloat3(&vectorPositionInput);
    XMVECTOR outputVector = XMLoadFloat3(&vectorPositionOutput);

    float length = XMVector3Length(inputVector - outputVector).m128_f32[0];

    XMFLOAT3 normalizedDistance;
    normalizedDistance.x = (vectorPositionInput.x - vectorPositionOutput.x) / length;
    normalizedDistance.y = (vectorPositionInput.y - vectorPositionOutput.y) / length;
    normalizedDistance.z = (vectorPositionInput.z - vectorPositionOutput.z) / length;


    normalizedDistance.x = abs(normalizedDistance.x);
    normalizedDistance.y = abs(normalizedDistance.y);
    normalizedDistance.z = abs(normalizedDistance.z);

    PRINT("Normalize");
    PRINT(normalizedDistance.x);
    PRINT(normalizedDistance.y);
    PRINT(normalizedDistance.z);

    

    return normalizedDistance.x <= 0 && normalizedDistance.y <= 0 && normalizedDistance.x <= 0;
}
void GameObjectManager::Update(Renderer* renderer) {
    HRESULT hr;

    //PRINT("Rendering...");

    renderer->Precommandlist();


    
    //PRINT("Frame");
    std::vector<std::pair<std::string, std::string>> testedPairs;

    //for (auto& pair : objectMap) {
    //    GameObject* gameObject = pair.second;
    //    gameObject->Update(1.0, renderer, m_pCamera);

    //    // try collide = object on which we test collide
    //    for (auto& tryCollide : objectMap) {
    //        if (tryCollide.first == pair.first) {
    //            continue;
    //        }



    //        std::pair<std::string, std::string> objectPair(pair.first, tryCollide.first);
    //        auto it = std::find(testedPairs.begin(), testedPairs.end(), objectPair);

    //        // not in the array
 
    //        XMFLOAT3 componentInputPosition = gameObject->GetComponent<Transform>(ComponentType::Transform)->GetPosition();
    //        XMFLOAT3 componentOutputPosition = tryCollide.second->GetComponent<Transform>(ComponentType::Transform)->GetPosition();

    //        //PRINT("PAS TESTE");
    //        //PRINT("Object1");
    //        //PRINT(tryCollide.first);
    //        //PRINT("Object2");
    //        //PRINT(pair.first);

    //        bool hey = IsColliding(componentInputPosition, componentOutputPosition);

    //        // Ajoutez la paire testée au vecteur

    //        testedPairs.push_back(objectPair);
    //    }
    //}

    for (const auto& pair : testedPairs) {
        std::cout << "Tested Pair: (" << pair.first << ", " << pair.second << ")\n";
    }

    // CHECK COLLIDING
    

    renderer->Postcommandlist();

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    hr = renderer->m_pSwapChain->Present(1, 0);
    renderer->m_frameIndex = (renderer->m_frameIndex + 1) % renderer->m_FRAME_COUNT;
    ASSERT_FAILED(hr);

    renderer->WaitForPreviousFrame();

    //PRINT("Rendering complete");
}