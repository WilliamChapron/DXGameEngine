#include "Engine.h"
#include <wrl/client.h>
#include "../include.h"   

using Microsoft::WRL::ComPtr;
using namespace DirectX;

bool InitializeDirectX12Resources(ComPtr<ID3D12Device>& pDevice, ComPtr<ID3D12CommandQueue>& pCommandQueue, ComPtr<ID3D12CommandAllocator>& pCommandAllocator) {
    // Créer le périphérique DirectX 12
    HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice));
    if (FAILED(hr)) {
        std::cerr << "Failed to create DirectX 12 device." << std::endl;
        return false;
    }

    std::cout << "Init" << std::endl;

    // Créer la file de commandes
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    hr = pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pCommandQueue));
    if (FAILED(hr)) {
        std::cerr << "Failed to create command queue." << std::endl;
        return false;
    }


    // Créer le command allocator
    hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAllocator));
    if (FAILED(hr)) {
        std::cerr << "Failed to create command allocator." << std::endl;
        return false;
    }

    // Tentative de réinitialisation du command allocator
    hr = pCommandAllocator->Reset();
    if (FAILED(hr)) {
        std::cerr << "Failed to reset command allocator." << std::endl;
        return false;
    }
    if (SUCCEEDED(hr)) {
        std::cout << "Success to reset command allocator." << std::endl;
        return false;
    }

    // Initialisation réussie
    return true;
}


XMVECTOR TranslateVector(XMVECTOR vector, float deltaX, float deltaY, float deltaZ) {
    // Création de la matrice de translation
    XMMATRIX translationMatrix = XMMatrixTranslation(deltaX, deltaY, deltaZ);

    // Application de la translation au vecteur
    return XMVector3Transform(vector, translationMatrix);
}


void Engine::Init(HINSTANCE hInstance, int nShowCmd) {
    _hInstance = hInstance;

    // Créer la console
    AllocConsole();

    if (freopen_s(&_pConsole, "CONOUT$", "w", stdout) != 0) {
        MessageBox(0, L"Failed to redirect console output", L"Error", MB_OK);
        Cleanup();
        PostQuitMessage(1);
    }

    // Initialiser la fenêtre
    Window::GetInstance().Init(hInstance, nShowCmd);

    ComPtr<ID3D12Device> pDevice;
    ComPtr<ID3D12CommandQueue> pCommandQueue;
    ComPtr<ID3D12CommandAllocator> pCommandAllocator;


    //// Initialiser les ressources DirectX 12
    //if (InitializeDirectX12Resources(pDevice, pCommandQueue, pCommandAllocator)) {
    //    // Utilisez les ressources DirectX 12 comme nécessaire
    //    // ...

    //    // Après utilisation, vous pouvez réinitialiser le command allocator si nécessaire
    //    HRESULT hr = pCommandAllocator->Reset();
    //    if (FAILED(hr)) {
    //        std::cerr << "Failed to reset command allocator." << std::endl;
    //        // Gérez l'erreur selon vos besoins
    //    }
    //}

    // Création d'un vecteur représentant la position initiale
    XMVECTOR initialPosition = XMVectorSet(1.0f, 2.0f, 3.0f, 1.0f);

    // Affichage de la position initiale
    std::cout << "Position initiale : (" << XMVectorGetX(initialPosition) << ", " << XMVectorGetY(initialPosition) << ", " << XMVectorGetZ(initialPosition) << ")\n";

    // Translation du vecteur
    XMVECTOR newPosition = TranslateVector(initialPosition, 1.0f, 2.0f, -3.0f);

    // Affichage de la nouvelle position
    std::cout << "Nouvelle position : (" << XMVectorGetX(newPosition) << ", " << XMVectorGetY(newPosition) << ", " << XMVectorGetZ(newPosition) << ")\n";

    // Lancer la boucle principale
    Run();
}

void Engine::Cleanup() {
    if (_pConsole) {
        fclose(_pConsole);
        FreeConsole();
    }
}

void Engine::Run() {
    std::cout << "Main Loop Started" << std::endl;


    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //else {
        //    Sleep(1000);
        //    std::cout << "sdsd" << std::endl;
        //}
    }
}
void Engine::Update() {
    // static instance

}
