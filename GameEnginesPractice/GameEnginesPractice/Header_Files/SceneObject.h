#pragma once
#include "Ogre.h"

#include "OgreItem.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2.h"

class SceneObject
{
public:
	SceneObject() = delete;
	SceneObject(Ogre::String name, Ogre::SceneNode* sceneNode);
	~SceneObject();

	void SetPosition(Ogre::Vector3 newPos);
	void Translate(Ogre::Vector3 offset);
	Ogre::Vector3 GetPosition() const;

	void SetScale(Ogre::Vector3 newScale);
	Ogre::Vector3 GetScale() const;

	Ogre::String GetName() const;

	void SetOrientation(Ogre::Quaternion orientation);
	void Rotate(Ogre::Quaternion rotation);
	Ogre::Quaternion GetOrientation() const;
	
private:
	Ogre::String m_strName;
	Ogre::SceneNode* m_pSceneNode;
};

