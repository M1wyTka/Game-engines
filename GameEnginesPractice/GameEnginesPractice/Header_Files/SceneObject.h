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
	SceneObject(Ogre::String name, Ogre::SceneNode* sceneNode);
	~SceneObject();

	void SO_SetPosition(Ogre::Vector3 newPos);
	Ogre::Vector3 SO_GetPosition();

	void SO_SetScale(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	Ogre::String GetName();

private:
	Ogre::String m_strName;
	Ogre::SceneNode* m_pSceneNode;
};

