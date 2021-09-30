#pragma once
#include "flecs.h"
#include "GeneralSystems.h"
#include "PhysSystems.h"
#include "OgreVector3.h"
#include "InputHandler.h"

struct Controllable 
{
	float ControllSpeed;
};

void LoadControlSystems(flecs::world& World);
