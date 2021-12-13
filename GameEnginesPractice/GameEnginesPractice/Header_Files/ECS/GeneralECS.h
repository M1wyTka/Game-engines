#pragma once
#include "flecs.h"
#include "OgreCamera.h"

struct InputHandlerPtr
{
	class InputHandler* ptr;
};

struct RenderEnginePtr
{
	class RenderEngine* ptr;
};

struct MainCameraPtr 
{
	class Ogre::Camera* ptr;
};

void LoadGeneralSystems(flecs::world& world);