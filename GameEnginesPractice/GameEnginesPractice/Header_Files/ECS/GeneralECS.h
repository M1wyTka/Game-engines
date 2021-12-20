#pragma once
#include "flecs.h"
#include "OgreCamera.h"
#include "ScriptSystem/Script.h"
#include "ScriptSystem/ScriptManager.h"
#include "Render/RenderEngine.h"

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

struct ScriptSystemPtr
{
	ScriptManager* ptr;
};


struct ScriptComponent
{
	Script* ptr;
};

void LoadGeneralSystems(flecs::world& world);