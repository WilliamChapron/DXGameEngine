// ScriptComponent.h
#pragma once

#include "../entities/GameObject.hpp"
#include "../systems/Input.h"
#include "../../core/Engine.h"
#include <iostream>
#include <ctime>

class ScriptComponent : public GameObject {
public:
    //ScriptComponentTester(); //Constructor
    //~ScriptComponentTester(); //and Destroyer 
    virtual void Update();


    void HelloPrintScript();
    void RandomNumberScript();
    void DateAndTimeScript();


private:
    
    //float m_Speed;
};



class ScriptComponentAttack : public ScriptComponent { 
private: 
    ;
public:
    //ScriptComponentAttack(); //Constructor
    //~ScriptComponentAttack(); //and Destructor
   
    

   
}; 


class ScriptComponentHealth : public ScriptComponent {
private:

public:
    void SetInitialHealth(float initialHealth) { 
        m_Health = initialHealth;
    }

    void TakeDamage(float damageAmount) {
        m_Health -= damageAmount;
    }

    void Heal(float healAmount) {
        m_Health += healAmount;
    }

private:
    float m_Health; // Initial health value
};





//Class dérviée de scriptCOmponent pour les health



//class ScriptComponent  {
//
//};



/* Fini pour la lib, finition d'input et du vaisseau côté game, script component pour gérer les scripts*/