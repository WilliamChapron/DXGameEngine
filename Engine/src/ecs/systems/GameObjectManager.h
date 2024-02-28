#pragma once

#include "../../include.h"
#include "../../core/Defines.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "../entities/GameObject.h"

using namespace std;

class Renderer;
class GameObject;


// Classe GameObject
class GameObjectManager {
private:

    std::unordered_map<std::string, GameObject> objectMap; // Map des objets avec leurs indices


public:
    GameObjectManager();

    // Ajouter un triangle au gestionnaire
    void AddObject(std::string name, const GameObject& objects);

    // Supprimer un triangle du gestionnaire
    void RemoveObject(std::string name);

    // Mettre à jour un triangle existant
    void UpdateObject(std::string name, const GameObject& newObject);

    // Méthode de rendu des triangles
    void Render(Renderer* renderer);

};