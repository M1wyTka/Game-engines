#include "PhysSystems.h"

void LoadPhysSystems(flecs::world& world) 
{
    world.system<const Velocity, const Mass>()
        .each([&](flecs::entity e, const Velocity& vel, const Mass& mass)
            {
                //pos += vel * e.delta_time();
            });
    
    world.system<Position, const Velocity>()
        .each([&](flecs::entity e, Position& pos, const Velocity& vel)
            {
                pos += vel * e.delta_time();
            });
}