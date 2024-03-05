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
//class Camera;

class ComponentManager
{
public:
    ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager, Renderer* renderer, Camera* camera);

    // Actions to add, remove, ... on a game object
    void AddComponent(GameObject& gameObject, Component* component);
    //void RemoveComponent(GameObject gameObject, Component& component);
    //void RemoveAllComponents(GameObject& gameObject);
    void UpdateComponents(GameObject* gameObject);

    Component* GetComponentByType(GameObject& gameObject, ComponentType componentType);

private:
    Camera* m_pCamera;

    //std::map<GameObject*, std::list<Component*>> componentsByGameObject;
    std::shared_ptr<GameObjectManager> m_pGameObjectManager;
    Renderer* m_pRenderer;
};
