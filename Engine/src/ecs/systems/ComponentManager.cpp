#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include <unordered_map>

// Pass Instance of GameObjectManager to Work with alive entity
ComponentManager::ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager, Renderer* renderer) : m_pGameObjectManager(gameObjectManager), m_pRenderer(renderer)
{
    //PRINT(m_pGameObjectManager.use_count());
}





void ComponentManager::AddComponent(GameObject& gameObject, Component* addComponent) {
    std::unordered_map<std::string, GameObject*> aliveObjects = m_pGameObjectManager->GetAliveObjects();

    // let's check if object is in game object manager
    //auto it = std::find_if(aliveObjects.begin(), aliveObjects.end(), [gameObject](const auto& pair) {
    //    return pair.second == gameObject;
    //});

    //if (it == aliveObjects.end()) {
    //    PRINT("Game object not in game object manager ");
    //    return;
    //}

    // Check if the component is already in the object
    for (const Component* component : gameObject.componentsList) {
        if (component->GetName() == addComponent->GetName()) {
            PRINT("Object already in ");
            return;
        }
    }

    PRINT("Push component ");
    gameObject.componentsList.push_back(addComponent);

    
    for (const Component* component : gameObject.componentsList) {
        std::cout << "    Component: " << component->GetName() << std::endl;
    }
}

void ComponentManager::UpdateComponents(GameObject& gameObject) {

    for (Component* component : gameObject.componentsList) {
        if (component->ShouldUpdate()) {
            component->Update(m_pRenderer);
        }
    }
}

Component* ComponentManager::GetComponentByType(GameObject& gameObject, ComponentType componentType) {
    for (Component* component : gameObject.componentsList) {
        if (component->GetType() == componentType) {
            return component;
        }
    }
    return nullptr;
}