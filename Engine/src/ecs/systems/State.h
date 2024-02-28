#pragma once
class State
{
public:
    virtual void enter() = 0; // Méthode appelée lors de l'entrée dans l'état
    virtual void update() = 0; // Méthode appelée lors de la mise à jour de l'état
    virtual void exit() = 0; // Méthode appelée lors de la sortie de l'état
};