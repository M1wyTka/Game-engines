#pragma once
#include "Ogre.h"

#include "OgreItem.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2.h"

#define IMPORT_NAME(x) x + " Imported" 

class SceneObject
{
public:
	SceneObject() = delete;
	SceneObject(Ogre::SceneManager& sceneManager, Ogre::String meshName);
	SceneObject(Ogre::String name, Ogre::SceneNode* sceneNode);
	SceneObject(Ogre::String itemName, Ogre::MeshPtr mesh);
	~SceneObject();

	void SO_SetPosition(Ogre::Vector3 newPos);
	Ogre::Vector3 SO_GetPosition();

	void SO_SetScale(Ogre::Real x, Ogre::Real y, Ogre::Real z);

private:
	Ogre::String m_strName;
	Ogre::MeshPtr v2Mesh;
	Ogre::SceneNode* m_pSceneNode;
	Ogre::Item* m_pItem;

private:
	void SO_LoadMeshModel(Ogre::String meshName);
	void SO_LoadManagerItems(Ogre::SceneManager& sceneManager);
};

