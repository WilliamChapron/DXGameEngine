#pragma once

#include "./include.h"
#include <iostream>
#include <iomanip>

//using namespace DirectX;

void PrintMatrix(const DirectX::XMFLOAT4X4& matrix);

void printFloatWithPrecision(float value, int precision);

std::wstring stringToWString(const std::string& narrowString);

HRESULT CompileShaderFromFile(const wchar_t* filePath, const char* entryPoint, const char* shaderModel, ID3DBlob** blob);
