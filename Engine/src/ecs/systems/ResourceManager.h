#pragma once


 

#include <iostream>
#include <map>


#include "../components/Component.h"  


class TextureComponent;
class Mesh;

struct TextureComponentInfo {
    int key;
    TextureComponent* component;
};

struct MeshComponentInfo {
    int key;
    Mesh* component;
};


class ResourceManager
{
public:
    ResourceManager();


    void AddTextureToResources(Component* addComponent);
    void AddMeshToResources(Mesh* addComponent);

    MeshComponentInfo FindMeshComponentByName(const std::string& componentName);
    TextureComponentInfo FindTextureComponentByName(const std::string & componentName);
        

private:

    // RESSOURCE MANAGER
    std::map<int, TextureComponent*> m_textureComponents;
    std::map<int, Mesh*> m_meshRendererComponents;
    //

    // Id
    int m_currentTextureComponentID;
    int m_currentMeshComponentID;
    //
};
