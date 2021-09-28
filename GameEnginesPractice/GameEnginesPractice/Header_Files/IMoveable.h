#pragma once
#include "Ogre.h"

class IMoveable 
{
public:
	IMoveable() :
		m_vecCurrentVelocity(Ogre::Vector3(0, 0, 0)),
		m_vecCurrentPos(Ogre::Vector3(0, 0, 0)),
		m_bIsMovable(true) 
	{}

	IMoveable(Ogre::Vector3 InitialPosition,
		Ogre::Vector3 InitialVelocity, bool IsMovable) :
		m_vecCurrentPos(InitialPosition),
		m_vecCurrentVelocity(InitialVelocity),
		m_bIsMovable(IsMovable)
	{}

	virtual Ogre::Vector3 GetVelocity() { return m_vecCurrentVelocity; }
	virtual void SetVelocity(Ogre::Vector3 newVel) { m_vecCurrentVelocity = newVel; }

	virtual Ogre::Vector3 GetPosition() { return m_vecCurrentPos; }
	virtual void SetPosition(Ogre::Vector3 newPos) { m_vecCurrentPos = newPos; }
	virtual void UpdatePosition(float timestep) = 0;

	virtual bool IsMovable() { return m_bIsMovable; }
	
protected:
	bool m_bIsMovable;
	Ogre::Vector3 m_vecCurrentVelocity;
	Ogre::Vector3 m_vecCurrentPos;
};