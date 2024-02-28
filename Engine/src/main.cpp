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
            PRINT("ESCAPE pressed!");
            running = false;
        }

        if (input.isKeyDown(VK_UP)) {
            //std::cout << "UP arrow key pressed" << std::endl;
            PRINT("UP arrow key pressed!"); //Checking if spacebar is pressed!
        }

        if (input.isMouseDown()) {
            //std::cout << "Mouse left button pressed!" << std::endl;
            PRINT("Mouse left button pressed!"); // Check si le clic gauche est préssé
        }
    }

    return 0;
}
   // Console::GetInstance().Init(); 
   // Console::GetInstance().WriteToConsole("Hello");


    //return 0;
//}

