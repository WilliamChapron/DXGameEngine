#include "Utils.h"
#include "core/Defines.h"

void PrintMatrix(const DirectX::XMFLOAT4X4& matrix)
{
    std::cout << std::fixed << std::setprecision(3);

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            std::cout << std::setw(8) << matrix.m[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}


void printFloatWithPrecision(float value, int precision) {
    std::cout << std::fixed << std::setprecision(precision) << value << std::endl;
}

std::wstring stringToWString(const std::string& narrowString) {
    // Conversion en chaîne de caractères large
    return std::wstring(narrowString.begin(), narrowString.end());
}

HRESULT CompileShaderFromFile(const wchar_t* filePath, const char* entryPoint, const char* shaderModel, ID3DBlob** blob)
{
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(filePath, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, blob, &errorBlob);
    ASSERT_FAILED(hr);

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(static_cast<const char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
        PRINT("Shader compilation failed");
        return hr;
    }

    if (errorBlob) errorBlob->Release();

    PRINT("Shader compilation successful");

    return S_OK;
}
