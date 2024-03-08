#include "Input.h"
#include "State.h"


Input::Input(WindowProperties _m_wndProps) : m_wndProps(&_m_wndProps){
    keyStates.clear();
    // Initialisation de l'�tat des touches clavier 
    // G�re les touches Z, Q, S, D
    for (char key = 'A'; key <= 'Z'; ++key) {
        keyStates[key] = KeyState::Inactive;
    }
    // G�re les touches Espace et Shift
    keyStates[VK_SPACE] = KeyState::Inactive;
    keyStates[VK_SHIFT] = KeyState::Inactive;
    // G�re les touches des fl�ches directionnelles
    keyStates[VK_UP] = KeyState::Inactive;
    keyStates[VK_DOWN] = KeyState::Inactive;
    keyStates[VK_LEFT] = KeyState::Inactive;
    keyStates[VK_RIGHT] = KeyState::Inactive;
    // G�re le bouton gauche de la souris
    keyStates[VK_LBUTTON] = KeyState::Inactive;
    // G�re le bouton droit de la souris
    keyStates[VK_RBUTTON] = KeyState::Inactive;
}

void Input::Update() {
    // Met � jour l'�tat des touches clavier
    HandleKeyboardInput();
    // G�re les clics de souris
    HandleMouseInput();
    // Trouve la position de la souris
    GetMousePosition();
}

void Input::HandleKeyboardInput() {
    // Stocke l'�tat de chaque touche dans keyStates
    // G�re les touches Z, Q, S, D
    for (char key = 'A'; key <= 'Z'; ++key) {
        keyStates[key] = GetKeyStateHelper(key);
    }
    // G�re les touches Espace et Shift
    keyStates[VK_SPACE] = GetKeyStateHelper(VK_SPACE);
    keyStates[VK_SHIFT] = GetKeyStateHelper(VK_SHIFT);
    // G�re les touches des fl�ches directionnelles
    keyStates[VK_UP] = GetKeyStateHelper(VK_UP);
    keyStates[VK_DOWN] = GetKeyStateHelper(VK_DOWN);
    keyStates[VK_LEFT] = GetKeyStateHelper(VK_LEFT);
    keyStates[VK_RIGHT] = GetKeyStateHelper(VK_RIGHT);
}

void Input::HandleMouseInput() {
    // V�rifie si le bouton gauche de la souris est enfonc�
    keyStates[VK_LBUTTON] = GetKeyStateHelper(VK_LBUTTON);

    // V�rifie si le bouton droit de la souris est enfonc�
    keyStates[VK_RBUTTON] = GetKeyStateHelper(VK_RBUTTON);
}

KeyState Input::GetKeyStateHelper(char key) const {
    // R�cup�re l'�tat actuel de la touche
    bool isKeyDown = GetAsyncKeyState(key) & 0x8000;
    // Recherche de l'�tat pr�c�dent de la touche dans keyStates
    auto search = keyStates.find(key);
    KeyState previousState = search->second;

    // Si la touche est enfonc�e 
    if (isKeyDown) {
        // Si la touche �tait enfonc�e avant
        if (previousState == KeyState::Pressed || previousState == KeyState::Held) {
            return KeyState::Held;
        }
        // Si la touche n'�tait pas enfonc�e avant
        else {
            return KeyState::Pressed;
        }
    }
    // Si la touche n'est pas enfonc�e 
    else {
        // Si la touche �tait enfonc�e avant
        if (previousState == KeyState::Pressed || previousState == KeyState::Held) {
            return KeyState::Released;
        }
        // Si la touche n'�tait pas enfonc�e avant
        else {
            return KeyState::Inactive;
        }
    }
}


KeyState Input::GetKeyState(char key) const {
    // Recherche la touche dans le unordered_map keyStates
    auto it = keyStates.find(key);
    // Si la touche est trouv�e, retourne son �tat
    if (it != keyStates.end()) {
        return it->second;
    }
    else {
        // Si la touche n'est pas trouv�e, retourne false (non press�e)
        return KeyState::Inactive;
    }
}

const std::unordered_map<char, KeyState>& Input::GetKeyStates() const {
    return keyStates;
}

RECT GetWindowPosition(HWND hwnd) {
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    return windowRect;
}

POINT Input::GetMousePosition() const {
    POINT cursorPos;
    GetCursorPos(&cursorPos); // Obtient la position actuelle du curseur
    ScreenToClient(m_wndProps->hwnd, &cursorPos);

    cursorPos.x -= (*m_wndProps).width / 2;
    cursorPos.y -= (*m_wndProps).height / 2;

    return cursorPos; // Retourne les coordonn�es actuelles de la souris
}

void Input::ResetMousePosition() {
    POINT center;
    RECT windowRect = GetWindowPosition(m_wndProps->hwnd);
    
    ClientToScreen(m_wndProps->hwnd, &center); // Convertit les coordonnées de la fenêtre aux coordonnées de l'écran
    
    center.x = (*m_wndProps).width / 2 + windowRect.left + 8;
    center.y = (*m_wndProps).height / 2 + windowRect.top + 31;

    //std::cout << "barre " << GetSystemMetrics(SM_CYCAPTION) << std::endl;

    SetCursorPos(center.x, center.y); // Réinitialise la position de la souris au centre de la fenêtre
}
