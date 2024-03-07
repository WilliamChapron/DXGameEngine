#include "ResourceManager.h"

#include "../components/Component.h"
#include "../components/Texture.h"
#include "../components/Mesh.h"
#include "../components/Shader.h"

ResourceManager::ResourceManager() : m_currentMeshComponentID(0), m_currentTextureComponentID(0), m_currentShaderComponentID(0) {
}

void ResourceManager::AddTextureToResources(TextureComponent* addComponent) {
    int componentID = ++m_currentTextureComponentID;
    // #TODO check si existe déja 
    m_textureComponents[componentID] = addComponent;
}

void ResourceManager::AddShaderToResources(ShaderComponent* addComponent) {
    int componentID = ++m_currentShaderComponentID;
    // #TODO check si existe déja 
    m_shaderComponents[componentID] = addComponent;
}

void ResourceManager::AddMeshToResources(Mesh* addComponent) {
    int componentID = ++m_currentMeshComponentID;
    // #TODO check si existe déja 
    m_meshRendererComponents[componentID] = addComponent;
}



MeshComponentInfo ResourceManager::FindMeshComponentByName(const std::string& componentName) {
    for (const auto& pair : m_meshRendererComponents) {
        int key = pair.first;
        Mesh* meshComponent = pair.second;
        if (meshComponent->GetName() == componentName) {
            return { key, meshComponent };
        }
    }

    return { -1, nullptr };
}

ShaderComponentInfo ResourceManager::FindShaderComponentByName(const std::string& componentName) {
    for (const auto& pair : m_shaderComponents) {
        int key = pair.first;
        ShaderComponent* shaderComponent = pair.second;
        if (shaderComponent->GetName() == componentName) {
            return { key, shaderComponent };
        }
    }

    return { -1, nullptr };
}

TextureComponentInfo ResourceManager::FindTextureComponentByName(const std::string& componentName) {
    for (const auto& pair : m_textureComponents) {
        int key = pair.first;
        const auto& component = pair.second;
        TextureComponent* textureComponent = dynamic_cast<TextureComponent*>(component);

        if (textureComponent->GetName() == componentName) {
            return { key, textureComponent };
        }
    }

    return { -1, nullptr };
}