#pragma once
#include "IMoveable.h"
#include "SceneObject.h"

class GameObject : public IMoveable
{
public:
	GameObject();

	GameObject(Ogre::Vector3 InitialPosition,
				Ogre::Vector3 InitialVelocity, bool IsMovable);

	GameObject(SceneObject* SceneObject);

	GameObject(SceneObject* SceneObject, Ogre::Vector3 InitialPosition,
				Ogre::Vector3 InitialVelocity, bool IsMovable);

	~GameObject();

	virtual void Update(float deltaTime) {};

	virtual void SetSceneObject(SceneObject* SceneObject) { m_pSceneObject.reset(SceneObject); };
	virtual SceneObject& GetSceneObject() { return *m_pSceneObject; };
	virtual Ogre::String GetName() { return m_pSceneObject->GetName(); }

protected:
	std::unique_ptr<SceneObject> m_pSceneObject;
};
