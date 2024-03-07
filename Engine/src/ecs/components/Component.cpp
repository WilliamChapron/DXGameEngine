#include "Component.h"

Component::Component(std::string componentName, ComponentType componentType) : m_name(componentName), m_type(componentType), m_hasChanged(true) {

}

Component::~Component() {

}


void Component::Update(Renderer* renderer)
{
}