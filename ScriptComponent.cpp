// ScriptComponent.cpp
#include "ScriptComponent.h"


void ScriptComponent::HelloPrintScript() {
    std::cout << "Hello, world!" << std::endl;
}

void ScriptComponent::RandomNumberScript() {
    srand(time(0)); // Seed for random number generator
    int randomNumber = rand() % 100; // Generate a random number between 0 and 99
    std::cout << "Random Number: " << randomNumber << std::endl;
}

//void ScriptComponentTester::DateAndTimeScript() {
//    time_t now = time(0);
//    char* dateTimeString = ctime(&now);
//    std::cout << "Current Date and Time: " << dateTimeString << std::endl;
//}




