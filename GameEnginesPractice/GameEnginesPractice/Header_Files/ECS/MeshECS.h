#pragma once
#include "flecs.h"
#include "GeneralECS.h"

struct SceneObj
{
	class SceneObject* pSceneObject;
};

struct MeshName
{
	Ogre::String name;
};

struct SceneObjectPattern 
{
	Ogre::String name;
	Ogre::String meshName;
	bool isVisible;
};

void LoadMeshSystems(flecs::world& world);

void LoadMeshCreationSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery);

void LoadMeshPositionsUpdateSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery);