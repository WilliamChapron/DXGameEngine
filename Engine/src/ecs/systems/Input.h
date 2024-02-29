#pragma once

#include <unordered_map>
#include <Windows.h>
#include "StateMachine.h" // Inclure votre StateMachine et State si nécessaire

class Input {
public:
    Input(StateMachine* stateMachine);

    // Met à jour l'état des entrées
    void Update();

    // Trouve l'état d'une touche en fonction de sa clé
    bool GetKeyState(char key) const;

    // Trouve l'état de toutes les touches
    const std::unordered_map<char, bool>& GetKeyStates() const;

    // Trouve la position de la souris
    POINT GetMousePosition() const; 

private:
    StateMachine* stateMachine; // Pointeur vers la StateMachine

    // Méthode pour gérer les entrées clavier
    void HandleKeyboardInput();

    // Méthode pour gérer les clics de souris
    void HandleMouseInput();

    // Stocke l'état des touches clavier
    std::unordered_map<char, bool> keyStates;
};
