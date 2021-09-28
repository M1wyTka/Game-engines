#include "Bullet.h"

Bullet::Bullet() : GameObject()
{
}

Bullet::Bullet(SceneObject* SceneObject, Ogre::Vector3 InitialPosition,
	Ogre::Vector3 InitialVelocity, bool IsMovable) :
	GameObject(SceneObject, InitialPosition, InitialVelocity, IsMovable)
{}

Bullet::Bullet(Ogre::Vector3 InitialPosition,
	Ogre::Vector3 InitialVelocity, bool IsMovable) :
	GameObject(InitialPosition, InitialVelocity, IsMovable)
{}

void Bullet::Update(float deltaTime)
{
	m_vecCurrentPos += m_vecCurrentVelocity * deltaTime;
}

void Bullet::UpdatePosition(float timestep)
{

}

Bullet::~Bullet()
{}