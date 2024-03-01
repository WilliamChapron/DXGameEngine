#pragma once

#include "../../include.h"
#include <iostream>



using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Renderer;

class Texture
{
public:
	Texture(std::string name);
	void Initialize(Renderer* renderer);

private:
	std::string name;
	ComPtr<ID3D12Resource> m_textureBuffer;
	ComPtr<ID3D12Resource> m_uploadTexture;
};

