#pragma once

#include "../../include.h"
#include "./Component.h"

#include <iostream>

class Renderer;

using namespace DirectX;
using Microsoft::WRL::ComPtr;


class ShaderComponent : public Component
{
public:
	ShaderComponent(std::string name, Renderer* pRenderer);
	~ShaderComponent();

	void InitializePSO();
	void InitializeRootSignature();
	void Update(Renderer* pRenderer) override;
	void UpdatePSO();
	void UpdateRootSignature();

private:

	Renderer* m_pRenderer;

	ComPtr<ID3DBlob> m_vertexShaderBlob;
	ComPtr<ID3DBlob> m_pixelShaderBlob;

	ComPtr<ID3D12PipelineState> m_pPipelineState;
	ComPtr<ID3D12RootSignature> m_pRootSignature;
	
};

