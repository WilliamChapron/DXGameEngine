#include "ResourceManager.h"

#include "../components/Component.h"
#include "../components/Texture.h"
#include "../components/MeshRenderer.h"

ResourceManager::ResourceManager() : m_currentMeshComponentID(0), m_currentTextureComponentID(0) {
}

int ResourceManager::AddTextureToResources(Component* addComponent) {
    // WORKING ON TEXTURE
    if (dynamic_cast<TextureComponent*>(addComponent)) {
        int componentID = ++m_currentTextureComponentID;
        // #TODO check si existe déja 
        m_textureComponents[componentID] = static_cast<TextureComponent*>(addComponent);
        return componentID; 
    }
    return 0;
}

int ResourceManager::AddMeshToResources(Component* addComponent) {
    if (dynamic_cast<MeshRenderer*>(addComponent)) {
        int componentID = ++m_currentMeshComponentID;
        m_meshComponents[componentID] = static_cast<MeshRenderer*>(addComponent);
        return componentID; 
    }
    return 0;
}

MeshRenderer* ResourceManager::FindMeshComponentByName(const std::string& componentName) {
    for (const auto& pair : m_meshComponents) {

        const auto& component = pair.second;
        MeshRenderer* meshComponent = dynamic_cast<MeshRenderer*>(component);

        if (meshComponent->GetName() == componentName) {
            return meshComponent;
        }
    }

    return nullptr;
}

TextureComponent* ResourceManager::FindTextureComponentByName(const std::string& componentName) {
    for (const auto& pair : m_textureComponents) {

        const auto& component = pair.second;
        TextureComponent* textureComponent = dynamic_cast<TextureComponent*>(component);

        if (textureComponent->GetName() == componentName) {
            return textureComponent;
        }
    }

    return nullptr;
}