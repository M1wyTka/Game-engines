#include "ECS/ControlECS.h"
#include "ECS/MeshECS.h"
#include "ECS/KinematicsECS.h"

void LoadControlSystems(flecs::world& world) 
{
    static auto renderQuery = world.query<RenderEnginePtr>();
    static auto inputQuery = world.query<InputHandlerPtr>();
    world.system<DeltaPos, const Controllable>()
        .each([&](flecs::entity e, DeltaPos& offset, const Controllable& ctr)
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
                            offset.val += deltaVel * e.delta_time();
                        });
                    }
                });
            });

    world.system<MainCameraPtr, const Controllable>()
        .kind(flecs::PostUpdate)
        .each([&](flecs::entity e, MainCameraPtr& cam, const Controllable& ctr)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        inputQuery.each([&](InputHandlerPtr input)
                            {
                                Ogre::Vector2 pressedDeltaMouse = -input.ptr->DeltaDownMousePos() * input.ptr->GetMouseSensitivity();
                                pressedDeltaMouse *= e.delta_time();
                                rendEngine.ptr->GetRT()->RC_LambdaAction([=] { 
                                        cam.ptr->move(Ogre::Vector3(pressedDeltaMouse.x, pressedDeltaMouse.y, 0));
                                        cam.ptr->lookAt(Ogre::Vector3(0, 0, 0));
                                    });
                            });
                    });
            });
}
