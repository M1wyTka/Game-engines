#pragma once

#include "SceneObject.h"

class SceneObjectProducer;

class SceneObjectProducer
{
public:
	SceneObjectProducer();
	SceneObjectProducer(Ogre::SceneManager* sceneManager);

	~SceneObjectProducer();

	SceneObject* Produce(Ogre::String actorName, Ogre::String meshName);

private:
	Ogre::SceneManager* m_pSceneManager;
	std::map<Ogre::String, int> m_mpUsedNames;
	std::map<Ogre::String, int> m_mpUsedMeshes;

	Ogre::MeshPtr LoadMeshModel(Ogre::String meshName, int instanceNum);
	Ogre::Item* LoadItem(Ogre::MeshPtr);
	Ogre::SceneNode* LoadNode(Ogre::Item* item);
};

