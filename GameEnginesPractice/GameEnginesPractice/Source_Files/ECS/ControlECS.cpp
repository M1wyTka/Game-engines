#include "ECS/ControlECS.h"
#include "ECS/MeshECS.h"

void LoadControlSystems(flecs::world& world) 
{
    static auto renderQuery = world.query<RenderEnginePtr>();
    static auto inputQuery = world.query<InputHandlerPtr>();
    world.system<Velocity, const Controllable>()
        .each([&](flecs::entity e, Velocity& vel, const Controllable& ctr)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                {
                    if (rendEngine.ptr->IsInitialized())
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

                            Ogre::Vector2 pressedDeltaMouse = -input.ptr->DeltaDownMousePos() * input.ptr->GetMouseSensitivity();

                            pressedDeltaMouse *= e.delta_time();
                            rendEngine.ptr->GetRT()->RC_MoveCamera(Ogre::Vector3(pressedDeltaMouse.x, pressedDeltaMouse.y, 0));
                        });
                    };
                });
            });
}
