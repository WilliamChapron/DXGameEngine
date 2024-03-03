#include "Input.h"
#include "State.h"


Input::Input() {
    keyStates.clear();
    // Initialisation de l'état des touches clavier
    // Gère les touches Z, Q, S, D
    keyStates['Z'] = KeyState::Inactive;
    keyStates['Q'] = KeyState::Inactive;
    keyStates['S'] = KeyState::Inactive;
    keyStates['D'] = KeyState::Inactive;
    // Gère les touches Espace et Shift
    keyStates[VK_SPACE] = KeyState::Inactive;
    keyStates[VK_SHIFT] = KeyState::Inactive;
    // Gère les touches des flèches directionnelles
    keyStates[VK_UP] = KeyState::Inactive;
    keyStates[VK_DOWN] = KeyState::Inactive;
    keyStates[VK_LEFT] = KeyState::Inactive;
    keyStates[VK_RIGHT] = KeyState::Inactive;
    // Gère le bouton gauche de la souris
    keyStates[VK_LBUTTON] = KeyState::Inactive;
    // Gère le bouton droit de la souris
    keyStates[VK_RBUTTON] = KeyState::Inactive;
}

void Input::Update() {
    // Met à jour l'état des touches clavier
    HandleKeyboardInput();
    // Gère les clics de souris
    HandleMouseInput();
    // Trouve la position de la souris
    GetMousePosition();
}

void Input::HandleKeyboardInput() {
    // Stocke l'état de chaque touche dans keyStates
    // Gère les touches Z, Q, S, D
    keyStates['Z'] = GetKeyStateHelper('Z');
    keyStates['Q'] = GetKeyStateHelper('Q');
    keyStates['S'] = GetKeyStateHelper('S');
    keyStates['D'] = GetKeyStateHelper('D');
    // Gère les touches Espace et Shift
    keyStates[VK_SPACE] = GetKeyStateHelper(VK_SPACE);
    keyStates[VK_SHIFT] = GetKeyStateHelper(VK_SHIFT);
    // Gère les touches des flèches directionnelles
    keyStates[VK_UP] = GetKeyStateHelper(VK_UP);
    keyStates[VK_DOWN] = GetKeyStateHelper(VK_DOWN);
    keyStates[VK_LEFT] = GetKeyStateHelper(VK_LEFT);
    keyStates[VK_RIGHT] = GetKeyStateHelper(VK_RIGHT);
}

void Input::HandleMouseInput() {
    // Vérifie si le bouton gauche de la souris est enfoncé
    keyStates[VK_LBUTTON] = GetKeyStateHelper(VK_LBUTTON);

    // Vérifie si le bouton droit de la souris est enfoncé
    keyStates[VK_RBUTTON] = GetKeyStateHelper(VK_RBUTTON);
}

KeyState Input::GetKeyStateHelper(char key) const {
    // Récupère l'état actuel de la touche
    bool isKeyDown = GetAsyncKeyState(key) & 0x8000;
    // Recherche de l'état précédent de la touche dans keyStates
    auto search = keyStates.find(key);
    KeyState previousState = search->second;

    // Si la touche est enfoncée 
    if (isKeyDown) {
        // Si la touche était enfoncée avant
        if (previousState == KeyState::Pressed || previousState == KeyState::Held) {
            return KeyState::Held;
        }
        // Si la touche n'était pas enfoncée avant
        else {
            return KeyState::Pressed;
        }
    }
    // Si la touche n'est pas enfoncée 
    else {
        // Si la touche était enfoncée avant
        if (previousState == KeyState::Pressed || previousState == KeyState::Held) {
            return KeyState::Released;
        }
        // Si la touche n'était pas enfoncée avant
        else {
            return KeyState::Inactive;
        }
    }
}


KeyState Input::GetKeyState(char key) const {
    // Recherche la touche dans le unordered_map keyStates
    auto it = keyStates.find(key);
    // Si la touche est trouvée, retourne son état
    if (it != keyStates.end()) {
        return it->second;
    }
    else {
        // Si la touche n'est pas trouvée, retourne false (non pressée)
        return KeyState::Inactive;
    }
}

const std::unordered_map<char, KeyState>& Input::GetKeyStates() const {
    return keyStates;
}

POINT Input::GetMousePosition() const {
    POINT cursorPos;
    GetCursorPos(&cursorPos); // Obtient la position actuelle du curseur
    return cursorPos; // Retourne les coordonnées actuelles de la souris
}