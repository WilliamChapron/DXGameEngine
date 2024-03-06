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
    void AddComponent(GameObject& gameObject, Component* component);




    void UpdateComponents(GameObject* gameObject);

    int AddTextureToResources(Component* addComponent);
    int AddMeshToResources(Component* addComponent);

    std::map<int, TextureComponent*> GetTextureComponents();

    Component* GetGameObjectComponentByType(GameObject& gameObject, ComponentType componentType);
    MeshRenderer* FindMeshComponentByName(const std::string& componentName);
    TextureComponent* FindTextureComponentByName(const std::string& componentName);

private:
    Camera* m_pCamera;

    // RESSOURCE MANAGER
    std::map<int, TextureComponent*> m_textureComponents;
    std::map<int, MeshRenderer*> m_meshComponents;
    //

    // Id
    int m_currentTextureComponentID;
    int m_currentMeshComponentID;
    //

    std::shared_ptr<GameObjectManager> m_pGameObjectManager;
    Renderer* m_pRenderer;
};
