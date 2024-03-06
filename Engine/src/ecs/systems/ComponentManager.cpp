#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../components/Camera.h"

#include "../components/Component.h"
#include "../components/Texture.h"
#include "../components/Mesh.h"
#include "MeshRenderer.h"


// Pass Instance of GameObjectManager to Work with alive entity
ComponentManager::ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager, Renderer* renderer, Camera* camera) : m_pGameObjectManager(gameObjectManager), m_pRenderer(renderer), m_pCamera(camera), m_currentMeshComponentID(0), m_currentTextureComponentID(0)
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



    //PRINT("Push component ");
    gameObject.componentsList.push_back(addComponent);

    
    for (const Component* component : gameObject.componentsList) {
        std::cout << "    Component: " << component->GetName() << std::endl;
    }

    PRINT("ds");
}

std::map<int, TextureComponent*> ComponentManager::GetTextureComponents() {
    return m_textureComponents;
}

void ComponentManager::UpdateComponents(GameObject* gameObject) {
    for (const auto& pair : gameObject->componentsList)
    {
        PRINT("pair->GetName()");
        PRINT(pair->GetName());
        //std::cout << pair->GetName() << " UPDATE !" << std::endl;
        pair->Update(m_pRenderer);
    }
}

Component* ComponentManager::GetGameObjectComponentByType(GameObject& gameObject, ComponentType componentType) {
    for (Component* component : gameObject.componentsList) {
        if (component->GetType() == componentType) {
            return component;
        }
    }
    return nullptr;
}

int ComponentManager::AddTextureToResources(Component* addComponent) {
    // WORKING ON TEXTURE
    if (dynamic_cast<TextureComponent*>(addComponent)) {
        int componentID = ++m_currentTextureComponentID;
        // #TODO check si existe déja 
        m_textureComponents[componentID] = static_cast<TextureComponent*>(addComponent);
        // #TODO AVOID DOUBLE COMPILATION
        return componentID; 
    }
    return 0;
}

int ComponentManager::AddMeshToResources(Component* addComponent) {
    if (dynamic_cast<MeshRenderer*>(addComponent)) {
        int componentID = ++m_currentMeshComponentID;
        m_meshComponents[componentID] = static_cast<MeshRenderer*>(addComponent);
        return componentID; 
    }
    return 0;
}

MeshRenderer* ComponentManager::FindMeshComponentByName(const std::string& componentName) {
    for (const auto& pair : m_meshComponents) {

        const auto& component = pair.second;
        MeshRenderer* meshComponent = dynamic_cast<MeshRenderer*>(component);

        if (meshComponent->GetName() == componentName) {
            return meshComponent;
        }
    }

    return nullptr;
}

TextureComponent* ComponentManager::FindTextureComponentByName(const std::string& componentName) {
    for (const auto& pair : m_textureComponents) {

        const auto& component = pair.second;
        TextureComponent* textureComponent = dynamic_cast<TextureComponent*>(component);

        if (textureComponent->GetName() == componentName) {
            return textureComponent;
        }
    }

    return nullptr;
}