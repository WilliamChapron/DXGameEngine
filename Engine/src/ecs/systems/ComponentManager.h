#pragma once


#include "../entities/GameObject.h"  
#include "../components/Component.h"  

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>


class GameObjectManager;

class ComponentManager
{
public:
    ComponentManager(std::shared_ptr<GameObjectManager>& gameObjectManager);

    // Actions to add, remove, ... on a game object
    void AddComponent(GameObject* gameObject, Component* component);
    //void RemoveComponent(GameObject gameObject, Component& component);
    //void RemoveAllComponents(GameObject& gameObject);


private:
    //std::map<GameObject*, std::list<Component*>> componentsByGameObject;
    std::shared_ptr<GameObjectManager> m_pGameObjectManager;
};
