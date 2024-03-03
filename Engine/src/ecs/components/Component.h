#pragma once

#include <iostream>

class Component {
public:
    Component(std::string componentName);
    ~Component();

    inline std::string GetName() const {
        return m_name;
    }


    virtual void Initialize();
private:
    std::string m_name;
};