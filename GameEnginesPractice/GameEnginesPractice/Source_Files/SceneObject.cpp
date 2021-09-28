#include "SceneObject.h"

SceneObject::SceneObject(Ogre::String name, Ogre::SceneNode* sceneNode) : m_strName(name), m_pSceneNode(sceneNode)
{
	m_pSceneNode->scale(0.1f, 0.1f, 0.1f);
}

SceneObject::~SceneObject()
{
}

void SceneObject::SO_SetScale(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_pSceneNode->scale(x, y, z);
}

void SceneObject::SO_SetPosition(Ogre::Vector3 newPos)
{
	m_pSceneNode->setPosition(newPos);
}

Ogre::String SceneObject::GetName() 
{
	return m_strName;
}

Ogre::Vector3 SceneObject::SO_GetPosition()
{
	return m_pSceneNode->getPosition();
}