#include "ECS/PhysECS.h"
#include "Universe.h"

void LoadPhysSystems(flecs::world& world) 
{
    world.system<Velocity, const Position, const Mass>()
        .iter([&](flecs::iter& it, Velocity* vel, const Position* pos, const Mass* mass)
            {
                for (int i : it)
                {
                    for (int j : it) 
                    {
                        if (i >= j)
                            continue;

                        Ogre::Vector3 posDifference = pos[j] - pos[i];
                        float sqrDistance = posDifference.squaredLength();

                        Ogre::Vector3 forceDir = posDifference.normalisedCopy();

                        Ogre::Vector3 accelerationI = forceDir * GravConst * mass[j].mass / sqrDistance;
                        Ogre::Vector3 accelerationII = -forceDir * GravConst * mass[i].mass / sqrDistance;

                        vel[i] += accelerationI * GravTimestep;
                        vel[j] += accelerationII * GravTimestep;
                    }
                }
            });
    
    world.system<Position, const Velocity>()
        .each([&](flecs::entity e, Position& pos, const Velocity& vel)
            {
                pos += vel * GravTimestep;
            });
}