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

struct CameraPosition : public Ogre::Vector3 
{
	using Ogre::Vector3::Vector3;
};

void LoadControlSystems(flecs::world& World);
