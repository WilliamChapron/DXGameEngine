#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "../../core/Defines.h"
#include <unordered_map>

// Pass Instance of GameObjectManager to Work with alive entity
ComponentManager::ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager) : m_pGameObjectManager(gameObjectManager)
{
    //PRINT(m_pGameObjectManager.use_count());
}



void ComponentManager::AddComponent(GameObject* gameObject, Component* component) {

    //gameObject->componentsList
    std::unordered_map<std::string, GameObject*> aliveObjects = m_pGameObjectManager->GetAliveObjects();

    // let's check if object is in game object manager
    auto it = std::find_if(aliveObjects.begin(), aliveObjects.end(), [gameObject](const auto& pair) {
        return pair.second == gameObject;
    });

    if (it == aliveObjects.end()) {
        PRINT("Game object not in game object manager ");
        return;
    }

    // Check if the component is already in the object
    for (const Component* component : gameObject->componentsList) {
        if (component->GetName() == component->GetName()) {
            PRINT("Object already in ");
            return;
        }
    }

    PRINT("Push component ");
    gameObject->componentsList.push_back(component);

    
    for (const Component* component : gameObject->componentsList) {
        std::cout << "    Component: " << component->GetName() << std::endl;
    }
}