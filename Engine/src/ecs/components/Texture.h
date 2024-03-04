#pragma once

#include "../../include.h"
#include "./Component.h"

#include <iostream>



using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Renderer;

class TextureComponent : public Component
{
public:
	TextureComponent(std::string name);
	void Initialize(Renderer* renderer);
	void Update(Renderer* renderer);

private:
	std::string m_namePath;
	ComPtr<ID3D12Resource> m_textureBuffer;
	ComPtr<ID3D12Resource> m_uploadTexture;
};

