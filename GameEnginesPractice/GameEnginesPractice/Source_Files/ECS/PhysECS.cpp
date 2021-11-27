#include "ECS/PhysECS.h"
#include "Universe.h"
#include "ECS/KinematicsECS.h"

void LoadPhysSystems(flecs::world& world)
{
    LoadPlanetGravitationSystem(world);
}

void LoadPlanetGravitationSystem(flecs::world& world)
{
    world.system<Velocity, const Position, DeltaPos, const Mass>()
        .kind(flecs::OnUpdate)
        .iter([&](flecs::iter& it, Velocity* vel, const Position* pos, DeltaPos* delta, const Mass* mass)
            {
                for (int i : it)
                {
                    for (int j : it)
                    {
                        if (i >= j)
                            continue;

                        Ogre::Vector3 posDifference = pos[j].val - pos[i].val;
                        float sqrDistance = posDifference.squaredLength();

                        Ogre::Vector3 forceDir = posDifference.normalisedCopy();
                        Ogre::Vector3 ampForce = forceDir * GravConst / sqrDistance;

                        Ogre::Vector3 accelerationI = ampForce * mass[j].mass;
                        Ogre::Vector3 accelerationII = -ampForce * mass[i].mass;

                        vel[i] += accelerationI * GravTimestep;
                        vel[j] += accelerationII * GravTimestep;

                        delta[i].val += vel[i] * GravTimestep;
                        delta[j].val += vel[j] * GravTimestep;
                    }
                }
            });
}