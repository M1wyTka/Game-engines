#pragma once
#include "flecs.h"
#include "Ogre.h"

struct Kinematics 
{
	Ogre::Quaternion Rotation;
	Ogre::Vector3    Position;
	Ogre::Vector3    Scale;	
};

struct DeltaKinematics 
{
	Ogre::Quaternion DeltaRot;
	Ogre::Vector3    DeltaPos;
	Ogre::Vector3    DeltaScale;
};

void LoadKinematicsSystems(flecs::world& world);