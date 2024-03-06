#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../components/Camera.h"

#include "../components/Component.h"
#include "../components/Texture.h"


// Pass Instance of GameObjectManager to Work with alive entity
ComponentManager::ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager, Renderer* renderer, Camera* camera) : m_pGameObjectManager(gameObjectManager), m_pRenderer(renderer), m_pCamera(camera), m_currentComponentID(0)
{
    //PRINT(m_pGameObjectManager.use_count());
}


int ComponentManager::AddTextureToResources(Component* addComponent) {
    // WORKING ON TEXTURE
    if (dynamic_cast<TextureComponent*>(addComponent)) {
        int componentID = ++m_currentComponentID;
        // #TODO check si existe déja 
        m_textureComponents[componentID] = static_cast<TextureComponent*>(addComponent);
        return componentID; // Offset??
    }
    return 0;
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

    AddTextureToResources(addComponent);
}

std::map<int, TextureComponent*> ComponentManager::GetTextureComponents() {
    return m_textureComponents;
}

void ComponentManager::UpdateComponents(GameObject* gameObject) {
    for (const auto& pair : gameObject->componentsList)
    {
        //std::cout << pair->GetName() << " UPDATE !" << std::endl;
        pair->Update(m_pRenderer);
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