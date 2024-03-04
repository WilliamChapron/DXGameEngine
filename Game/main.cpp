#include "src/include.h"
#include "src/core/Engine.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Rediriger la sortie standard vers la console avant d'initialiser l'Engine




    Engine::GetInstance().Init(hInstance, nShowCmd);

  


    //Console::GetInstance().Init();
    //Console::GetInstance().WriteToConsole("Hello");



    return 0;
}


