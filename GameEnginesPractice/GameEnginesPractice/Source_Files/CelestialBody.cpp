#include "CelestialBody.h"
#include "Universe.h"

CelestialBody::CelestialBody() : GameObject(), m_nMass(1)
{
}

CelestialBody::~CelestialBody()
{
}

void CelestialBody::SetBodyParameters(int mass, Ogre::Vector3 InitialPosition, Ogre::Vector3 InitialVelocity, bool IsMovable)
{
	m_nMass = mass;
	m_vecCurrentPos = InitialPosition;
	m_vecCurrentVelocity = InitialVelocity;
	m_bIsMovable = IsMovable;
}

void CelestialBody::UpdateVelocity(std::vector<std::unique_ptr<CelestialBody>>& allBodies, float timeStep)
{
	if (!m_bIsMovable)
		return;

	for (auto& body : allBodies) 
	{
		if (body.get() == this)
			continue;
		Ogre::Vector3 posDifference = body->GetPosition() - GetPosition();
		float sqrDistance = posDifference.squaredLength();
		Ogre::Vector3 forceDir = posDifference.normalisedCopy();

		Ogre::Vector3 acceleration = forceDir * GravConst * body->GetMass() / sqrDistance;
		m_vecCurrentVelocity += acceleration * timeStep;
	}
}

void CelestialBody::UpdatePosition(float timeStep) 
{
	if (!m_bIsMovable)
		return;

	m_vecCurrentPos += m_vecCurrentVelocity * timeStep;
}
