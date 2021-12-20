#include "ScriptSystem/EntityManager.h"
#include "ECS/GeneralECS.h"
#include "Ogre.h"

EntityManager::EntityManager(RenderEngine* pRenderEngine, ScriptManager* pScriptManager, flecs::world* ecs) :
	m_pRenderEngine(pRenderEngine),
	m_pEcs(ecs),
	m_pScriptManager(pScriptManager)
{

}

EntityManager::~EntityManager()
{
	
}

void EntityManager::CreateEntity(std::string strScriptName, SceneObjectPattern pat, Kinematics kin)
{
	flecs::entity newEntity = m_pEcs->entity();

	Script* pScript = m_pScriptManager->CreateScript(strScriptName, newEntity);

	newEntity.set(ScriptComponent{ pScript })
			 .set(Kinematics(kin))
			 .set(SceneObjectPattern(pat));;
	//newEntity.set(Scene{ sceneNode });
}