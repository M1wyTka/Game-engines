#include "SceneObject.h"

SceneObject::SceneObject(Ogre::String name, Ogre::SceneNode* sceneNode) : m_strName(name), m_pSceneNode(sceneNode)
{
	m_pSceneNode->scale(0.1f, 0.1f, 0.1f);
	m_pSceneNode->setName(name);
}

SceneObject::~SceneObject()
{
}

void SceneObject::SetScale(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_pSceneNode->scale(x, y, z);
}

void SceneObject::SetPosition(Ogre::Vector3 newPos)
{
	m_pSceneNode->setPosition(newPos);
}

void SceneObject::SetOrientation(Ogre::Quaternion newOrientation) 
{
	m_pSceneNode->setOrientation(newOrientation);
}

Ogre::Vector3 SceneObject::GetScale() const
{
	return m_pSceneNode->getScale();
}

Ogre::Vector3 SceneObject::GetPosition() const
{
	return m_pSceneNode->getPosition();
}

Ogre::Quaternion SceneObject::GetOrientation() const
{
	return m_pSceneNode->getOrientation();
}

Ogre::String SceneObject::GetName() const
{
	return m_strName;
}