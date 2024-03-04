#include "Mesh.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"

MeshComponent::MeshComponent(std::string name) : Component(name, ComponentType::Mesh, false)
{
    int a = 0;
}

void MeshComponent::Initialize(Renderer* renderer)
{
    
}

void MeshComponent::Update(Renderer* renderer) {
    PRINT("Update Mesh");
}