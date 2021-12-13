#include "ECS/PhysECS.h"
#include "Universe.h"
#include "ECS/KinematicsECS.h"
#include "OgreLogManager.h"

void LoadPhysSystems(flecs::world& world)
{
    LoadPlanetGravitationSystem(world);
}

void LoadPlanetGravitationSystem(flecs::world& world)
{
    world.system<const Kinematics, const Mass, Velocity, DeltaKinematics>()
        .kind(flecs::OnUpdate)
        .iter([&](flecs::iter& it, const Kinematics* kins, const Mass* mass, Velocity* vel, DeltaKinematics* dKins)
            {
                for (int i : it)
                {
                    for (int j : it)
                    {
                        if (i >= j)
                            continue;

                        Ogre::Vector3 posDifference = kins[j].Position - kins[i].Position;
                        float sqrDistance = posDifference.squaredLength();

                        Ogre::Vector3 forceDir = posDifference.normalisedCopy();
                        Ogre::Vector3 ampForce = forceDir * GravConst / sqrDistance;

                        Ogre::Vector3 accelerationI = ampForce * mass[j].mass;
                        Ogre::Vector3 accelerationII = -ampForce * mass[i].mass;

                        vel[i] += accelerationI * GravTimestep;
                        vel[j] += accelerationII * GravTimestep;

                        dKins[i].DeltaPos += vel[i] * GravTimestep;
                        dKins[j].DeltaPos += vel[j] * GravTimestep;
                    }
                }
            });
    {
        world.system<const Kinematics, const SphereCollider>()
            .kind(flecs::OnValidate)
            .iter([&](flecs::iter& it, const Kinematics* kins, const SphereCollider* collider)
                {
                    for (int i : it)
                    {
                        for (int j : it)
                        {
                            if (i >= j)
                                continue;

                            Ogre::Vector3 diff = kins[i].Position - kins[j].Position;
                            float  colDist = collider[i].radius + collider[j].radius;
                            if (diff.squaredLength() <= colDist*colDist)
                            {
                                Ogre::LogManager::getSingleton().logMessage("collision!");
                            }
                        }
                    }
                }); 
    }
}