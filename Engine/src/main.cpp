// main.cpp

#include <iostream>  
#include "Input.h"
#include "include.h"   
#include "core/Window.h"
#include "core/Engine.h"
#include "core/defines.h" 

#include <iostream>





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Rediriger la sortie standard vers la console avant d'initialiser l'Engine

    //Engine::GetInstance().Init(hInstance, nShowCmd); 
    Input input;

    bool running = true;
    while (running) {
        input.update();
        PRINT("Checking input");

        if (input.isKeyDown(VK_ESCAPE)) {
            running = false;
        }

        if (input.isKeyPressed(VK_SPACE)) {
            std::cout << "SPaceBar!" << std::endl;
            PRINT("Spacebar pressed!"); //Checking if spacebar is pressed!
        }
    }

    return 0;
}
    //Console::GetInstance().Init();
    //Console::GetInstance().WriteToConsole("Hello");



    //return 0;
//}

