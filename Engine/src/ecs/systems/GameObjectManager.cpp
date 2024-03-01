#include "GameObjectManager.h"
#include "../../renderer/Graphics.h" // renderer
#include "../components/Camera.h"

// Constructeur par défaut
GameObjectManager::GameObjectManager() {}

// Ajouter un objet au gestionnaire
void GameObjectManager::AddObject(std::string name, const GameObject& object) {
    objectMap[name] = object;
    PRINT("Object added");
    std::cout << name << std::endl;
}

// Supprimer un objet du gestionnaire
void GameObjectManager::RemoveObject(std::string name) {
    objectMap.erase(name);
    PRINT("Object erased");
}


//std::vector<GameObject> GameObjectManager::FindObjectsByName(const std::string& name) {
//    std::vector<GameObject> matchingObjects;
//
//    for (auto& pair : objectMap) {
//        if (pair.first == name) {
//            matchingObjects.push_back(pair.second);
//        }
//    }
//
//    return matchingObjects;
//}

void GameObjectManager::Update(Renderer* renderer, Camera* camera) {
    HRESULT hr;

    PRINT("Rendering...");

    renderer->Precommandlist();
    
    for (auto& pair : objectMap) {
        GameObject& gameObject = pair.second;
        gameObject.Update(1.0, renderer, camera);
    }



    renderer->Postcommandlist();

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    hr = renderer->m_pSwapChain->Present(1, 0);
    renderer->m_frameIndex = (renderer->m_frameIndex + 1) % renderer->m_FRAME_COUNT;
    ASSERT_FAILED(hr);

    renderer->WaitForPreviousFrame();

    PRINT("Rendering complete");
}
