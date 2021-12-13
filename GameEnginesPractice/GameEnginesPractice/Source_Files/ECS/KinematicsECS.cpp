#include "ECS/KinematicsECS.h"

void LoadKinematicsSystems(flecs::world& world) 
{
	{
		world.system<DeltaKinematics>()
			.kind(flecs::PreUpdate)
			.each([&](flecs::entity e, DeltaKinematics& dk)
				{
					dk.DeltaRot = Ogre::Quaternion::IDENTITY;
					dk.DeltaPos = Ogre::Vector3(0, 0, 0);
					dk.DeltaScale = Ogre::Vector3(0, 0, 0);
				});
	}

	{
		world.system<Kinematics, DeltaKinematics>()
			.kind(flecs::PostUpdate)
			.each([&](flecs::entity e, Kinematics& k, DeltaKinematics& dk)
				{
					k.Position += dk.DeltaPos;
					//k.Rotation += dk.DeltaRot;
					k.Scale += dk.DeltaScale;
				});
	}
}