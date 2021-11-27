#include "OGRE/OgreVector3.h"
#include "flecs.h"

struct Position
{
	Ogre::Vector3 val;
};

struct Rotation
{
	Ogre::Quaternion val;
};

struct Scale 
{
	Ogre::Vector3 val;
};

struct DeltaPos
{
	Ogre::Vector3 val;
};

struct DeltaScale
{
	Ogre::Vector3 val;
};

struct DeltaRotation
{
	Ogre::Quaternion val;
};

void LoadKinematicsSystems(flecs::world& world);