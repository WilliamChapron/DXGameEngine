#include "GameObjectManager.h"
#include "../../renderer/Graphics.h" // renderer

// Constructeur par défaut
GameObjectManager::GameObjectManager(Camera* camera) : m_pCamera(camera){}

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

void GameObjectManager::Update(Renderer* renderer) {
    HRESULT hr;

    //PRINT("Rendering...");

    renderer->Precommandlist();

    for (auto& pair : objectMap) {
        GameObject* gameObject = pair.second;
        gameObject->Update(1.0, renderer, m_pCamera);
    }

    renderer->Postcommandlist();

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    hr = renderer->m_pSwapChain->Present(1, 0);
    renderer->m_frameIndex = (renderer->m_frameIndex + 1) % renderer->m_FRAME_COUNT;
    ASSERT_FAILED(hr);

    renderer->WaitForPreviousFrame();

    //PRINT("Rendering complete");
}
