#include "Component.h"

Component::Component(std::string componentName, ComponentType componentType, bool allowMultipleInstances) : m_name(componentName), m_type(componentType), m_hasChanged(true) {

}

Component::~Component() {

}

void Component::Initialize(Renderer* renderer) {

}

void Component::Update(Renderer* renderer)
{
}