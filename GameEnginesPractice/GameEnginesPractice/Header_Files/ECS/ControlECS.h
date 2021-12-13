#pragma once
#include "flecs.h"
#include "GeneralECS.h"
#include "PhysECS.h"

#include "AdjacentSystems/InputHandler.h"

#include "OgreVector3.h"

struct Controllable 
{
	float ControllSpeed;
};

void LoadControlSystems(flecs::world& World);

void GetWASDVector(InputHandler* input, Ogre::Vector2 &vec);
