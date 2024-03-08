#pragma once

#include <iostream>

class Renderer;

enum class ComponentType {
    Transform = 0,
    Shader = 1,
    Texture = 2,
    MeshRenderer = 3,
    ColliderComponent = 4
};

class Component {
public:
    Component(std::string componentName, ComponentType componentType);
    ~Component();

    inline std::string GetName() const {
        return m_name;
    }

    inline ComponentType GetType() const {
        return m_type;
    }

    // Return if component need update

    inline bool HasHeChanged() const {
        return m_hasChanged;
    }

    inline void SetHasChanged(bool value) {
        m_hasChanged = value; 
    }

    //virtual void Initialize(Renderer* renderer);
    virtual void Update(Renderer* renderer);

private:
    std::string m_name;
    ComponentType m_type;
    bool m_hasChanged; // Update if true
    //bool m_allowMultipleInstances; // Restrict creation with same name if true
};