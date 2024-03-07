#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../components/Camera.h"

#include "../components/Component.h"
#include "../components/Texture.h"
#include "../components/Mesh.h"
#include "../components/MeshRenderer.h"

#include <iostream>
#include <map>
#include <algorithm>


// Pass Instance of GameObjectManager to Work with alive entity
ComponentManager::ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager, Renderer* renderer, Camera* camera) : m_pGameObjectManager(gameObjectManager), m_pRenderer(renderer), m_pCamera(camera)
{
    //PRINT(m_pGameObjectManager.use_count());
}



bool CompareByPriority(const Component* leftValue, const Component* rightValue) {
    ComponentType leftType = leftValue->GetType();
    ComponentType rightType = rightValue->GetType();
    return static_cast<int>(leftType) < static_cast<int>(rightType); // Get value associate at an enum
}

void ComponentManager::SortComponentsMap(GameObject& gameObject) {
    // Transférer les composants dans un vecteur
    std::vector<Component*> componentsVector(gameObject.componentsList.begin(), gameObject.componentsList.end());

    // Trier le vecteur par priorité
    std::stable_sort(componentsVector.begin(), componentsVector.end(), CompareByPriority);

    // Remettre les composants triés dans la liste
    gameObject.componentsList.assign(componentsVector.begin(), componentsVector.end());
}


void ComponentManager::AddComponent(GameObject& gameObject, Component* addComponent) {

    // Check if the component is already in the object
    for (const Component* component : gameObject.componentsList) {
        if (component->GetName() == addComponent->GetName()) {
            PRINT("Object already in ");
            return;
        }
    }
    gameObject.componentsList.push_back(addComponent);
    SortComponentsMap(gameObject);
}

void ComponentManager::UpdateComponents(GameObject* gameObject) {
    for (const auto& pair : gameObject->componentsList)
    {
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

