#include "GameObject.h"
#include "renderer/Graphics.h" // renderer

// Constructeur par défaut
GameObject::GameObject() {}

// Ajouter un objet au gestionnaire
void GameObject::AddObject(std::string index, const GameObject& object) {
    objectMap[index] = object;
    PRINT("Object added");
    std::cout << index << std::endl;
}

// Supprimer un objet du gestionnaire
void GameObject::RemoveObject(std::string index) {
    objectMap.erase(index);
    PRINT("Object erased");
}

// Mettre à jour un objet existant
void GameObject::UpdateObject(std::string index, const GameObject& newObject) {
    objectMap[index] = newObject;
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
