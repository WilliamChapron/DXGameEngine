#pragma once

#include "../../include.h"
#include "../../core/Defines.h"
#include "../entities/GameObject.h"
#include <vector>
#include <unordered_map>
#include <string>

class Renderer;
class Camera;

class GameObjectManager {
public:
    GameObjectManager(Camera* camera);

    void AddObject(const std::string& name, GameObject* object);
    void RemoveObject(const std::string& name);
    void Update(Renderer* renderer);

    inline const std::unordered_map<std::string, GameObject*>& GetAliveObjects() const {
        return objectMap;
    }

private:
    Camera* m_pCamera;

    std::unordered_map<std::string, GameObject*> objectMap;
};
