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
	void Initialize(Renderer* renderer, int offset);
	void Update(Renderer* renderer) override;

private:
	int m_offset;
	std::string m_namePath;
	ComPtr<ID3D12Resource> m_textureBuffer;
	ComPtr<ID3D12Resource> m_uploadTexture;

	CD3DX12_GPU_DESCRIPTOR_HANDLE m_textureAdress;
};

