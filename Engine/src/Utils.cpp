#include "Utils.h"

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
    return std::wstring(narrowString.begin(), narrowString.end());
}
