#pragma once


//#include "../entities/GameObject.h"  
#include "../components/Component.h"  

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include "../components/Camera.h"


class GameObjectManager;
class GameObject;
class Renderer;
class TextureComponent;
class Mesh;
class MeshRenderer;


class ComponentManager
{
public:
    ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager, Renderer* renderer, Camera* camera);

    // Actions to add, remove, ... on a game object
    inline ComponentType GetComponentPriority(Component* component) {
        return component->GetType();
    }
    void SortComponentsMap(GameObject& gameObject);
    void AddComponent(GameObject& gameObject, Component* component);


    void UpdateComponents(GameObject* gameObject);




    Component* GetGameObjectComponentByType(GameObject& gameObject, ComponentType componentType);

private:
    Camera* m_pCamera;

    std::shared_ptr<GameObjectManager> m_pGameObjectManager;
    Renderer* m_pRenderer;
};
