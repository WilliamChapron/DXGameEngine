#pragma once

#include <unordered_map>
#include <Windows.h>
#include <iostream>

enum class KeyState {
    Pressed,
    Held,
    Released,
    Inactive
};

class Input {
public:
    Input();

    // Met à jour l'état des entrées 
    void Update();

    // Trouve l'état d'une touche en fonction de sa clé
    KeyState GetKeyState(char key) const;

    // Trouve l'état de toutes les touches
    const std::unordered_map<char, KeyState>& GetKeyStates() const;

    // Trouve la position de la souris
    POINT GetMousePosition() const; 

private:

    // Méthode pour gérer les entrées clavier
    void HandleKeyboardInput();

    // Méthode pour gérer les clics de souris
    void HandleMouseInput();

    // Vérifie l'état de la touche spécifiée par sa clé
    KeyState GetKeyStateHelper(char key) const;

    // Stocke l'état des touches clavier
    std::unordered_map<char, KeyState> keyStates;
};
