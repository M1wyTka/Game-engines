#include "ControlSystems.h"


void LoadControlSystems(flecs::world& world) 
{
    static auto inputQuery = world.query<InputHandlerPtr>();
    world.system<Velocity, const Controllable>()
        .each([&](flecs::entity e, Velocity& vel, const Controllable& ctr)
            {
                inputQuery.each([&](InputHandlerPtr input)
                    {
                        
                        Ogre::Vector3 deltaVel = Ogre::Vector3::Vector3(0, 0, 0);
                        if (input.ptr->GetInputState().test(eIC_GoLeft))
                            deltaVel -= Ogre::Vector3::Vector3(ctr.ControllSpeed, 0, 0);
                        if (input.ptr->GetInputState().test(eIC_GoRight))
                            deltaVel += Ogre::Vector3::Vector3(ctr.ControllSpeed, 0, 0);
                        if (input.ptr->GetInputState().test(eIC_GoDown))
                            deltaVel -= Ogre::Vector3::Vector3(0, ctr.ControllSpeed, 0);
                        if (input.ptr->GetInputState().test(eIC_GoUp))
                            deltaVel += Ogre::Vector3::Vector3(0, ctr.ControllSpeed, 0);
                        vel += deltaVel * e.delta_time();
                    });
            });
}
