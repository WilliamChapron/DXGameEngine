#pragma once


 

#include <iostream>
#include <map>


#include "../components/Component.h"  


class TextureComponent;
class Mesh;
class ShaderComponent;

struct TextureComponentInfo {
    int key;
    TextureComponent* component;
};

struct MeshComponentInfo {
    int key;
    Mesh* component;
};

struct ShaderComponentInfo {
    int key;
    ShaderComponent* component;
};



class ResourceManager
{
public:
    ResourceManager();


    void AddTextureToResources(TextureComponent* addComponent);
    void AddShaderToResources(ShaderComponent* addComponent);
    void AddMeshToResources(Mesh* addComponent);


    ShaderComponentInfo FindShaderComponentByName(const std::string& componentName);
    MeshComponentInfo FindMeshComponentByName(const std::string& componentName);
    TextureComponentInfo FindTextureComponentByName(const std::string & componentName);
        

private:

    // RESSOURCE MANAGER
    std::map<int, TextureComponent*> m_textureComponents;
    std::map<int, Mesh*> m_meshRendererComponents; // Not a component
    std::map<int, ShaderComponent*> m_shaderComponents;
    //

    // Id
    int m_currentTextureComponentID;
    int m_currentMeshComponentID;
    int m_currentShaderComponentID;
    //
};
