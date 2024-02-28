#pragma once


#include "../include.h"

using namespace DirectX;

class Renderer;
struct Vertex;

void CreateIndexBuffer(const UINT& size, UINT* indices, ID3D12Resource* buffer, D3D12_INDEX_BUFFER_VIEW& bufferView, Renderer* renderer);
   

void CreateVertexBuffer(const UINT& size, Vertex* vertices, ID3D12Resource* buffer, D3D12_VERTEX_BUFFER_VIEW& bufferView, const UINT& stride, Renderer* renderer);
