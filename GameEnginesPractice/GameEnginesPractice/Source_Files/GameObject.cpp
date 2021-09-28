#include "GameObject.h"


GameObject::GameObject() : IMoveable(), m_pSceneObject(nullptr)
{

}

GameObject::GameObject(Ogre::Vector3 InitialPosition, Ogre::Vector3 InitialVelocity, bool IsMovable) :
	IMoveable(InitialPosition, InitialVelocity, IsMovable),
	m_pSceneObject(nullptr)
{
	
}

GameObject::GameObject(SceneObject* SceneObject) :
	IMoveable(),
	m_pSceneObject(SceneObject)
{

}

GameObject::GameObject(SceneObject* SceneObject, Ogre::Vector3 InitialPosition,
	Ogre::Vector3 InitialVelocity, bool IsMovable):
	IMoveable(InitialPosition, InitialVelocity, IsMovable),
	m_pSceneObject(SceneObject)
{

}

GameObject::~GameObject()
{
}
