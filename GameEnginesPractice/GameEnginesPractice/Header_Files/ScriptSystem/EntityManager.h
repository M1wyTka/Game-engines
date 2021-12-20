#pragma once

#include <string>
#include "flecs.h"
#include "Render/RenderEngine.h"
#include "ScriptManager.h"
#include "ECS/MeshECS.h"
#include "ECS/KinematicsECS.h"

class EntityManager
{
public:
	EntityManager(RenderEngine* pRenderEngine, ScriptManager* pScriptSystem, flecs::world* ecs);
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	void CreateEntity(std::string strScriptName, SceneObjectPattern pat, Kinematics kin);


private:
	ScriptManager* m_pScriptManager;
	RenderEngine* m_pRenderEngine;
	flecs::world* m_pEcs;
};