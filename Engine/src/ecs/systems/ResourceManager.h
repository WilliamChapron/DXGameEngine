#pragma once


 

#include <iostream>
#include <map>


#include "../components/Component.h"  


class TextureComponent;
class MeshRenderer;


class ResourceManager
{
public:
    ResourceManager();


    int AddTextureToResources(Component* addComponent);
    int AddMeshToResources(Component* addComponent);

    MeshRenderer* FindMeshComponentByName(const std::string& componentName);
    TextureComponent* FindTextureComponentByName(const std::string & componentName);
        

private:

    // RESSOURCE MANAGER
    std::map<int, TextureComponent*> m_textureComponents;
    std::map<int, MeshRenderer*> m_meshComponents;
    //

    // Id
    int m_currentTextureComponentID;
    int m_currentMeshComponentID;
    //
};
