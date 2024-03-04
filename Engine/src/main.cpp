// main.cpp

#include <iostream>  
#include "include.h"   
#include "core/Window.h"
#include "core/Engine.h"







int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Rediriger la sortie standard vers la console avant d'initialiser l'Engine



    
    Engine::GetInstance().Init(hInstance, nShowCmd);
    //




    //Console::GetInstance().Init();
    //Console::GetInstance().WriteToConsole("Hello");



    return 0;
}


