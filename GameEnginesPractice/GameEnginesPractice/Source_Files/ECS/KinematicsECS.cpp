#include "ECS/KinematicsECS.h"

void LoadKinematicsSystems(flecs::world& world) 
{
	{
		world.system<DeltaPos>().kind(flecs::PreUpdate)
			.each([&](flecs::entity e, DeltaPos& pos)
				{
					pos.val = Ogre::Vector3(0, 0, 0);
				});

		world.system<DeltaRotation>().kind(flecs::PreUpdate)
			.each([&](flecs::entity e, DeltaRotation& rot)
				{
					rot.val = Ogre::Quaternion(0, 0, 0, 0);
				});

		world.system<DeltaScale>().kind(flecs::PreUpdate)
			.each([&](flecs::entity e, DeltaScale& sc)
				{
					sc.val = Ogre::Vector3(0, 0, 0);
				});
	}

	{
		world.system<Position, DeltaPos>().kind(flecs::PostUpdate)
			.each([&](flecs::entity e, Position& pos, DeltaPos& delta)
				{
					pos.val += delta.val;
				});

		world.system<Rotation, DeltaRotation>().kind(flecs::PostUpdate)
			.each([&](flecs::entity e, Rotation& rot, DeltaRotation& delta)
				{
					//rot.val = Ogre::Quaternion(0, 0, 0, 0);
				});

		world.system<Scale, DeltaScale>().kind(flecs::PostUpdate)
			.each([&](flecs::entity e, Scale& sc, DeltaScale& delta)
				{
					sc.val = delta.val;
				});
	}
}