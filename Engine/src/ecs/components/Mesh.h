#pragma once

#include "../../include.h"
#include "./Component.h"


class Renderer;

class MeshComponent : public Component
{
public:
	MeshComponent(std::string name);
	void Initialize(Renderer* renderer);
	void Update(Renderer* renderer);

private:
	
};

