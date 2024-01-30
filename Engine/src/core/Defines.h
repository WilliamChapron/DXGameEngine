#include <iostream>

// Macro LOG
#ifdef DEBUG
#define LOG(message) std::cout << message << std::endl;
#else
#define LOG(message)
#endif

// Macro de condition pour le succès
#define CHECK_SUCCESS(hr, pointer) (SUCCEEDED(hr) && pointer != nullptr)

// Macro de condition pour l'échec
#define CHECK_FAILURE(hr, pointer) (FAILED(hr) || pointer == nullptr)

// Macro de log pour le succès
#define LOG_SUCCESS(name, verb) std::cout << "Success to " << verb << " " << name << "." << std::endl;

// Macro de log pour l'échec
#define LOG_FAILURE(name, verb) std::cout << "Failed to " << verb << " " << name << "." << std::endl;


// Macro pour print
#define PRINT(x) std::cout << x << std::endl
