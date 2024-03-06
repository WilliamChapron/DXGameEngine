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

void GameObject::Initialize(Renderer* renderer, Camera* camera, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale, std::string textureName) {
    PRINT("Creation des component");
    // Initialisation de l'objet GameObject avec un gestionnaire de composants
    Transform* defaultTransform = new Transform(position, rotation, scale);  // Création d'un objet Transform par défaut

    // Mise à jour des matrices de transformation du GameObject
    m_cbData.model = defaultTransform->GetTransformMatrix();  // Obtention de la matrice de transformation du GameObject
    m_cbData.view = camera->GetViewMatrix();  // Obtention de la matrice de vue de la caméra
    m_cbData.projection = camera->GetProjectionMatrix();  // Obtention de la matrice de projection de la caméra






    // Initialisation des composants Mesh et Texture

    Vertex cubeVertices[] = {
            { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
            { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
            { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
            { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { { 0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f} },
            { { 0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f} }
    };

    // Définition des indices du cube
    UINT cubeIndices[] = {
        0, 1, 2,
        2, 1, 3,
        4, 6, 5,
        6, 7, 5,
        0, 2, 4,
        2, 6, 4,
        1, 5, 3,
        3, 5, 7,
        2, 3, 6,
        3, 7, 6,
        0, 4, 1,
        1, 4, 5
    };

    Vertex* pVertices = &cubeVertices[0];
    UINT* pIndices = &cubeIndices[0];

    int numElementsV = sizeof(cubeVertices) / sizeof(cubeVertices[0]);
    int numElementsI = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

    TextureComponent* defaultTexture = new TextureComponent(textureName);
    MeshComponent* defaultMesh = new MeshComponent("Mesh", &m_cbData);

    // Add to resource manager before init
    int textureComponentID = m_pComponentManager->AddTextureToResources(defaultTexture);

    PRINT("COmponentIDPRINT");
    PRINT(textureComponentID);





    defaultTexture->Initialize(renderer, textureComponentID);  // Initialisation du composant Texture
    defaultMesh->Initialize(renderer, cubeVertices, numElementsV, cubeIndices, numElementsI);


    // Ajout des composants au gestionnaire de composants
    m_pComponentManager->AddComponent(*this, defaultTransform);  // Ajout du composant Transform au gestionnaire
    m_pComponentManager->AddComponent(*this, defaultTexture);  // Ajout du composant Texture au gestionnaire
    m_pComponentManager->AddComponent(*this, defaultMesh);  // Ajout du composant Mesh au gestionnaire

    PRINT("Suivant");

}

void GameObject::Update(Renderer* renderer, Camera* camera)
{
    Engine& e = Engine::GetInstance();  // Obtention de l'instance du moteur
    Component* component = e.m_pComponentManager->GetComponentByType(*this, ComponentType::Transform);  // Obtention du composant Transform de l'objet
    Transform* transformComponent = dynamic_cast<Transform*>(component);  // Conversion du composant en type Transform

    // Mise à jour des matrices de vue et de projection de la caméra
    m_cbData.view = camera->GetViewMatrix();  // Obtention de la matrice de vue de la caméra
    m_cbData.projection = camera->GetProjectionMatrix();  // Obtention de la matrice de projection de la caméra

    std::cout << "Update GameObject" << std::endl;  // Affichage de "Update" dans la console

    float rotationAngle = 0.03;  // Définition de l'angle de rotation
    float rotationOffset = 0.01;  // Définition du décalage de rotation
    float translationOffset = transformComponent->GetPosition().z + 0.01f;  // Définition du décalage de translation
    float fScale = transformComponent->GetScale().z - 0.001f;  // Définition de l'échelle de transformation

    transformComponent->Rotate(0, 0, rotationAngle);  // Rotation du composant Transform
    m_cbData.model = transformComponent->GetTransformMatrix();  // Mise à jour de la matrice de transformation du GameObject

    m_pComponentManager->UpdateComponents(this);  // Mise à jour des composants du GameObject
}