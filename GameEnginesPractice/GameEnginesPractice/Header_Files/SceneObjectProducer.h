#pragma once

#include "SceneObject.h"
#include "ECS/flecs.h"

class SceneObjectProducer;

class SceneObjectProducer
{
public:
	SceneObjectProducer();
	SceneObjectProducer(Ogre::SceneManager* sceneManager);

	~SceneObjectProducer();

	SceneObject* Produce(Ogre::String actorName, Ogre::String meshName);
	SceneObject* Produce(Ogre::String actorName);
	//flecs::entity* GetEntity(Ogre::SceneNode* node) { return m_pLogicVisualMapping[node]; }

private:
	Ogre::SceneManager* m_pSceneManager;
	std::map<Ogre::String, int> m_mpUsedNames;
	std::map<Ogre::String, int> m_mpUsedMeshes;
	//std::map<Ogre::SceneNode*, flecs::entity*> m_pLogicVisualMapping;

	Ogre::MeshPtr LoadMeshModel(Ogre::String meshName, int instanceNum);
	Ogre::Item* LoadItem(Ogre::MeshPtr);
	Ogre::SceneNode* LoadNode(Ogre::Item* item);
};

