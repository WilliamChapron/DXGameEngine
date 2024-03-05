#include "GameObject.h"
#include "../../renderer/Graphics.h"
#include "../../core/Defines.h"
#include "../../Utils.h"

#include "../components/Component.h"  
#include "../systems/ComponentManager.h"  

#include "../../core/Engine.h"  
#include "../components/Camera.h"

#include <stdexcept>

#include <DirectXColors.h>

#include "../components/Texture.h"
#include "../components/Transform.h"
#include "../components/Mesh.h"

#include "../../renderer/Resources.h"



#include <iostream>
#include <iomanip>


GameObject::GameObject(ComponentManager* componentManager) : m_cbData(), m_pComponentManager(componentManager)
{
}

void GameObject::Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale) {
    // Initialisation de l'objet GameObject avec un gestionnaire de composants
    Transform* defaultTransform = new Transform(position, rotation, scale);  // Création d'un objet Transform par défaut
  
    // Mise à jour des matrices de transformation du GameObject
    m_cbData.model = defaultTransform->GetTransformMatrix();  // Obtention de la matrice de transformation du GameObject
    m_cbData.view = camera->GetViewMatrix();  // Obtention de la matrice de vue de la caméra
    m_cbData.projection = camera->GetProjectionMatrix();  // Obtention de la matrice de projection de la caméra

    // Initialisation des composants Mesh, Texture et Shader par défaut
    MeshComponent* defaultMesh = new MeshComponent("Mesh", &m_cbData);  // Création d'un composant Mesh par défaut
    TextureComponent* defaultTexture = new TextureComponent("Texture");  // Création d'un composant Texture par défaut

    // Initialisation des composants Mesh et Texture
    defaultMesh->Initialize(renderer);  // Initialisation du composant Mesh
    defaultTexture->Initialize(renderer);  // Initialisation du composant Texture

    // Ajout des composants au gestionnaire de composants
    m_pComponentManager->AddComponent(*this, defaultTransform);  // Ajout du composant Transform au gestionnaire
    m_pComponentManager->AddComponent(*this, defaultMesh);  // Ajout du composant Mesh au gestionnaire
    m_pComponentManager->AddComponent(*this, defaultTexture);  // Ajout du composant Texture au gestionnaire
}


template <typename T>
T* GameObject::GetComponent(ComponentType type) {
    Component* component = m_pComponentManager->GetComponentByType(*this, type);
    return dynamic_cast<T*>(component);
}



void GameObject::Update(Renderer* renderer, Camera* camera)
{
//---------- William
    HRESULT hr;

    
    Transform* transformComponent = this->GetComponent<Transform>(ComponentType::Transform);

    m_cbData.view = camera->GetViewMatrix();
    m_cbData.projection = camera->GetProjectionMatrix();



    float rotationAngle = 90.0;
    float rotationOffset = 0.01;
    //PRINT("Translation offset");
    //PRINT(m_transform.vPosition.z);
    float translationOffset = transformComponent->GetPosition().z + 0.01f;
    float fScale = transformComponent->GetScale().z - 0.001f;
    //printFloatWithPrecision(m_transform.vScale.z, 4);


    //m_transform.Translate(m_transform.vPosition.x, m_transform.vPosition.y, translationOffset);
    transformComponent->Rotate(0.003, 0, 0);
    //printFloatWithPrecision(transformComponent->GetRotation().z,1);
    //m_transform.Scale(fScale, fScale, fScale);
//----------
    Engine& e = Engine::GetInstance();  // Obtention de l'instance du moteur
    Component* component = e.m_pComponentManager->GetComponentByType(*this, ComponentType::Transform);  // Obtention du composant Transform de l'objet


    // Mise à jour des matrices de vue et de projection de la caméra
    //m_cbData.view = camera->GetViewMatrix();  // Obtention de la matrice de vue de la caméra
    //m_cbData.projection = camera->GetProjectionMatrix();  // Obtention de la matrice de projection de la caméra

    std::cout << "Update GameObject" << std::endl;  // Affichage de "Update" dans la console


    //m_cbData.model = transformComponent->GetTransformMatrix();  // Mise à jour de la matrice de transformation du GameObject

    m_pComponentManager->UpdateComponents(this);  // Mise à jour des composants du GameObject
}