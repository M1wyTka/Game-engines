#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	Bullet(SceneObject* SceneObject, Ogre::Vector3 InitialPosition,
			Ogre::Vector3 InitialVelocity, bool IsMovable);
	Bullet(Ogre::Vector3 InitialPosition,
		Ogre::Vector3 InitialVelocity, bool IsMovable);
	~Bullet();

	void Update(float deltaTime);
	void UpdatePosition(float timestep);

private:

};
