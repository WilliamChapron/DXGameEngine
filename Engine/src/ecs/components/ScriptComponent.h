// ScriptComponent.h
#pragma once

#include "../entities/GameObject.hpp"
#include "../systems/Input.h"
#include "../../core/Engine.h"

class ScriptComponent : public Engine {
public:
    void Update();



private:
    void HandleInput() {
        KeyState keyZ = m_pInput->GetKeyState('Z');
        if (keyZ == KeyState::Held || keyZ == KeyState::Pressed) {
            m_pCamera->UpdatePosition(-0.02f, 0.0f, 0.0f); // A modifié avec la meilleure caméra et les bonnes coordonnées
        }

        KeyState keyQ = m_pInput->GetKeyState('Q');
        if (keyQ == KeyState::Held || keyQ == KeyState::Pressed) {
            m_pCamera->UpdatePosition(-0.0f, 0.05f, 0.0f); // A modifié avec la meilleure caméra et les bonnes coordonnées
        }

        KeyState keyS = m_pInput->GetKeyState('S');
        if (keyS == KeyState::Held || keyS == KeyState::Pressed) {
            m_pCamera->UpdatePosition(0.2f, 0.0f, 0.0f); // A modifié avec la meilleure caméra et les bonnes coordonnées
        }

        KeyState keyD = m_pInput->GetKeyState('D');
        if (keyD == KeyState::Held || keyD == KeyState::Pressed) {
            m_pCamera->UpdatePosition(-0.0f, 0.0f, 0.05f); // A modifié avec la meilleure caméra et les bonnes coordonnées
        }
    }
    //float m_Speed;
};

//class ScriptComponent  {
//
//};



/* Fini pour la lib, finition d'input et du vaisseau côté game, script component pour gérer les scripts*/