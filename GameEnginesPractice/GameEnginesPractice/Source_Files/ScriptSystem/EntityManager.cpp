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

	DeltaKinematics dk = { Ogre::Quaternion(Ogre::Quaternion::IDENTITY), Ogre::Vector3(0.f, 0.f, 0.f), Ogre::Vector3(1.f, 1.f, 1.f) };

	pScript->SetKinematics(kin);
	pScript->SetDeltaKinematics(dk);
	pScript->InitScript();
	pScript->LoadComponents(newEntity);

	newEntity.set(ScriptComponent{ pScript })
			 .set(Kinematics(kin))
			 .set(DeltaKinematics(dk))
			 .set(SceneObjectPattern(pat));;
	//newEntity.set(Scene{ sceneNode });
}