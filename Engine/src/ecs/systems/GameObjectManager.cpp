#include "GameObjectManager.h"
#include "../../renderer/Graphics.h" // renderer

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

// Mettre à jour un objet existant
//void GameObjectManager::UpdateObject(std::string name, const GameObject& newObject) {
//    objectMap[name] = newObject;
//    PRINT("Object updated");
//}

// Méthode de rendu des objets
void GameObjectManager::Update(Renderer* renderer) {
    // Boucle à travers tous les objets et appeler leur méthode de rendu individuelle
    HRESULT hr;

    PRINT("Rendering...");

    renderer->Precommandlist();
    
    for (auto& pair : objectMap) {
        GameObject& gameObject = pair.second;
        // Appeler la méthode de mise à jour de chaque objet
        gameObject.Update(1.0, renderer); // Supposons que chaque objet a une méthode Update
    }

    //triangle->Update(1.0, this);

    renderer->Postcommandlist();

    ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList.Get() };
    renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    hr = renderer->m_pSwapChain->Present(1, 0);
    renderer->m_frameIndex = (renderer->m_frameIndex + 1) % renderer->m_FRAME_COUNT;
    ASSERT_FAILED(hr);

    renderer->WaitForPreviousFrame();

    PRINT("Rendering complete");
}
