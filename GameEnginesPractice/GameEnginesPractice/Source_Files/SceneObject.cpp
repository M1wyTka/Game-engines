#include "SceneObject.h"

SceneObject::SceneObject(Ogre::String name, Ogre::SceneNode* sceneNode) : m_strName(name), m_pSceneNode(sceneNode)
{
	m_pSceneNode->scale(0.1f, 0.1f, 0.1f);
	m_pSceneNode->setName(name);
}

SceneObject::~SceneObject()
{
}

void SceneObject::SetScale(Ogre::Vector3 newScale)
{
	m_pSceneNode->scale(newScale);
}

void SceneObject::SetPosition(Ogre::Vector3 newPos)
{
	m_pSceneNode->setPosition(newPos);
}

void SceneObject::Translate(Ogre::Vector3 offset)
{
	m_pSceneNode->translate(offset);
}

void SceneObject::SetOrientation(Ogre::Quaternion newOrientation) 
{
	m_pSceneNode->setOrientation(newOrientation);
}

void SceneObject::Rotate(Ogre::Quaternion rotation)
{
	m_pSceneNode->rotate(rotation);
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

void SceneObject::SetVisibility(bool newState) 
{
	m_pSceneNode->setVisible(newState);
}

Ogre::String SceneObject::GetMeshName() 
{
	return (static_cast<Ogre::Item*> (m_pSceneNode->getAttachedObject(0)))->getMesh()->getName();
}