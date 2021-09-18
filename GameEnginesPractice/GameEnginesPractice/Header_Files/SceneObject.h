#pragma once
#include "Ogre.h"
#include "OgreRoot.h"
#include "OgreWindow.h"
#include "OgreItem.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2.h"

#define IMPORT_NAME(x) x " Imported" 

class SceneObject
{
public:
	SceneObject() = delete;
	SceneObject(Ogre::SceneManager* sceneManager);
	~SceneObject();

	void SO_SetPosition(Ogre::Vector3 newPos);

private:
	Ogre::MeshPtr v2Mesh;
	Ogre::SceneManager* m_pSceneManager;
	Ogre::SceneNode* m_pSceneNode;
	Ogre::Item* m_pItem;

private:
	void SO_LoadMeshModel(Ogre::String meshName);
	void SO_LoadManagerItems();
};

