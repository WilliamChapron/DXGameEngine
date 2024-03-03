#pragma once

#include "./include.h"
#include <iostream>

using namespace DirectX;

void PrintMatrix(const DirectX::XMFLOAT4X4& matrix);

void printFloatWithPrecision(float value, int precision);

std::wstring stringToWString(const std::string& narrowString);
