#pragma once
#include "flecs.h"
#include "GeneralECS.h"
#include "PhysECS.h"

#include "OgreString.h"
#include "OgreVector3.h"

#include "Render/RenderEngine.h"

struct SceneNode
{
	class Ogre::SceneNode* pSceneNode;
};

struct SceneObj
{
	class SceneObject* pSceneObject;
};

struct MeshName
{
	Ogre::String name;
};

struct SceneNodeDeliveryIndex
{
	uint32_t idx;
};

struct Scale : public Ogre::Vector3
{
	using Ogre::Vector3::Vector3;
};

void LoadMeshSystems(flecs::world& world);

void LoadMeshCreationSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery);

void LoadMeshDeliverySystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery);

void LoadMeshPositionsUpdateSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery);