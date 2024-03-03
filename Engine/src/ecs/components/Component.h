#pragma once

#include <iostream>

enum class ComponentType {
    Shader,
    Mesh,
    Texture,
    Transform
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


    virtual void Initialize();
private:
    std::string m_name;
    ComponentType m_type;
};