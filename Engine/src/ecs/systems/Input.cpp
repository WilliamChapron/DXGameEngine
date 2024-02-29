#include "Input.h"
#include "State.h"

class Pressed : public State {
public:
    virtual void enter() override {};
    virtual void update() override {};
    virtual void exit() override {};
};

class Held : public State {
public:
    virtual void enter() override {};
    virtual void update() override {};
    virtual void exit() override {};
};

class Released : public State {
public:
    virtual void enter() override {};
    virtual void update() override {};
    virtual void exit() override {};
};

class Inactive : public State {
public:
    virtual void enter() override {};
    virtual void update() override {};
    virtual void exit() override {};
};

Input::Input(StateMachine* stateMachine) : m_pStateMachine(stateMachine) {
    m_pStateMachine->AddState(new Pressed);
    m_pStateMachine->AddState(new Held);
    m_pStateMachine->AddState(new Released);
    m_pStateMachine->AddState(new Inactive);

    // Initialisation de l'état des touches clavier
    keyStates.clear();
}

void Input::Update() {
    // Met à jour l'état des touches clavier
    HandleKeyboardInput();
    // Gère les clics de souris
    HandleMouseInput();
    // Trouve la position de la souris
    GetMousePosition();
    // Mise à jour de l'état actuel de la StateMachine
    m_pStateMachine->UpdateCurrentState();
}

void Input::HandleKeyboardInput() {
    // Efface l'état des touches avant la mise à jour
    keyStates.clear();

    // Stock l'état de chaque touche dans keyStates
    // Gère les touches Z, Q, S, D
    keyStates['Z'] = GetAsyncKeyState('Z') & 0x8000;
    keyStates['Q'] = GetAsyncKeyState('Q') & 0x8000;
    keyStates['S'] = GetAsyncKeyState('S') & 0x8000;
    keyStates['D'] = GetAsyncKeyState('D') & 0x8000;
    // Gère les touches Espace et Shift
    keyStates[VK_SPACE] = GetAsyncKeyState(VK_SPACE) & 0x8000;
    keyStates[VK_SHIFT] = GetAsyncKeyState(VK_SHIFT) & 0x8000;
    // Gère les touches des flèches directionnelles
    keyStates[VK_UP] = GetAsyncKeyState(VK_UP) & 0x8000;
    keyStates[VK_DOWN] = GetAsyncKeyState(VK_DOWN) & 0x8000;
    keyStates[VK_LEFT] = GetAsyncKeyState(VK_LEFT) & 0x8000;
    keyStates[VK_RIGHT] = GetAsyncKeyState(VK_RIGHT) & 0x8000;

}

void Input::HandleMouseInput() {

    // Vérifie si le bouton gauche de la souris est enfoncé
    keyStates[VK_LBUTTON] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

    // Vérifie si le bouton droit de la souris est enfoncé
    keyStates[VK_RBUTTON] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
}

// Fonction pour obtenir l'état d'une touche spécifique
bool Input::GetKeyState(char key) const {
    // Recherche la touche dans le unordered_map keyStates
    auto it = keyStates.find(key);
    // Si la touche est trouvée, retourne son état
    if (it != keyStates.end()) {
        return it->second;
    }
    else {
        // Si la touche n'est pas trouvée, retourne false (non pressée)
        return false;
    }
}

const std::unordered_map<char, bool>& Input::GetKeyStates() const {
    return keyStates;
}

POINT Input::GetMousePosition() const {
    POINT cursorPos;
    GetCursorPos(&cursorPos); // Obtient la position actuelle du curseur
    return cursorPos; // Retourne les coordonnées actuelles de la souris
}