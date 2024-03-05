#include "Mesh.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "Component.h"


MeshComponent::MeshComponent(std::string name) : Component(name, ComponentType::Mesh, false)
{
}

void MeshComponent::Initialize(Renderer* renderer)
{

}

void MeshComponent::Update(Renderer* renderer) {
    PRINT("Update Mesh");
}