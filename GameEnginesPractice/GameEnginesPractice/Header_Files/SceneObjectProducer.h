#pragma once

#include "SceneObject.h"

class SceneObjectProducer;

struct LoadedObject 
{
	SceneObject* object;
	Ogre::String initialName;
	Ogre::String initialMeshName;
};

class SceneObjectProducer
{
public:
	SceneObjectProducer();
	SceneObjectProducer(Ogre::SceneManager* sceneManager);

	~SceneObjectProducer();

	SceneObject* Produce(Ogre::String actorName, Ogre::String meshName);
	SceneObject* Produce(Ogre::String actorName);
	//flecs::entity* GetEntity(Ogre::SceneNode* node) { return m_pLogicVisualMapping[node]; }
	std::vector<LoadedObject*>* GetLoadedObjects() { return &m_pLoadedObjects; }

private:
	Ogre::SceneManager* m_pSceneManager;
	std::map<Ogre::String, int> m_mpUsedNames;
	std::map<Ogre::String, int> m_mpUsedMeshes;
	//std::map<Ogre::SceneNode*, flecs::entity*> m_pLogicVisualMapping;
	std::vector<LoadedObject*> m_pLoadedObjects;
	std::map<Ogre::String, Ogre::String> m_mpNameMeshName;

	Ogre::MeshPtr LoadMeshModel(Ogre::String meshName, int instanceNum);
	Ogre::Item* LoadItem(Ogre::MeshPtr);
	Ogre::SceneNode* LoadNode(Ogre::Item* item);
};

