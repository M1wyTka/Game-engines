#pragma once
#include <OgreVector3.h>
#include "flecs.h"

struct Position : public Ogre::Vector3
{
	using Ogre::Vector3::Vector3;
};

struct Velocity : public Ogre::Vector3
{
	using Ogre::Vector3::Vector3;
};

struct Orientation : public Ogre::Quaternion 
{
	using Ogre::Quaternion::Quaternion;
};

struct Mass
{
	float mass;
};

void LoadPhysSystems(flecs::world& world);