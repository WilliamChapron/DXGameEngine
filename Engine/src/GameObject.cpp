#include "GameObject.h"
#include "renderer/Graphics.h" // renderer

// Constructeur par défaut
GameObject::GameObject() {}

// Ajouter un objet au gestionnaire
void GameObject::AddObject(std::string name, const GameObject& object) {
    objectMap[name] = object;
    PRINT("Object added");
    std::cout << name << std::endl;
}

// Supprimer un objet du gestionnaire
void GameObject::RemoveObject(std::string name) {
    objectMap.erase(name);
    PRINT("Object erased");
}

// Mettre à jour un objet existant
void GameObject::UpdateObject(std::string name, const GameObject& newObject) {
    objectMap[name] = newObject;
    PRINT("Object updated");
}

// Méthode de rendu des objets
void GameObject::Render(Renderer* renderer) {
    // Boucle à travers tous les objets et appeler leur méthode de rendu individuelle
    for (auto& pair : objectMap) {
        pair.second.Render(renderer);
    }
    PRINT("Object rendered");
}
