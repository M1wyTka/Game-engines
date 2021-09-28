#pragma once
#include "Ogre.h"
#include "SceneObject.h"
#include "GameObject.h"

class CelestialBody : public GameObject
{
public:
	CelestialBody();

	~CelestialBody();

	void SetBodyParameters(int mass = 1, Ogre::Vector3 InitialPosition = Ogre::Vector3(0, 0, 0),
							Ogre::Vector3 InitialVelocity = Ogre::Vector3(0, 0, 0), bool IsMovable = true);
	
	void UpdateVelocity(std::vector<std::unique_ptr<CelestialBody>> &allBodies, float timeStep);
	void UpdatePosition(float timeStep) override;

	Ogre::Real GetMass() { return m_nMass; }
	
	Ogre::String GetName() { return m_sName; }
	SceneObject& GetActor() { return *m_pSceneObject; }

private:
	Ogre::String m_sName;
	int m_nMass;
};

