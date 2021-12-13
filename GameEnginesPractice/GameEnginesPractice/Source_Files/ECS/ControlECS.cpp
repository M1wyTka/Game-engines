#include "ECS/ControlECS.h"
#include "ECS/MeshECS.h"
#include "ECS/KinematicsECS.h"
#define _USE_MATH_DEFINES
#include <math.h>

void LoadControlSystems(flecs::world& world) 
{
    static auto renderQuery = world.query<RenderEnginePtr>();
    static auto inputQuery = world.query<InputHandlerPtr>();
    world.system<DeltaKinematics, const Controllable>()
        .each([&](flecs::entity e, DeltaKinematics& offset, const Controllable& ctr)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                {
                    if (rendEngine.ptr->IsInitialized())
                    {
                        inputQuery.each([&](InputHandlerPtr input)
                        {
                            Ogre::Vector2 inputVec;
                            GetWASDVector(input.ptr, inputVec);
                            Ogre::Vector3 deltaVel = ctr.ControllSpeed * Ogre::Vector3(inputVec.x, inputVec.y, 0) * e.delta_time();
                            offset.DeltaPos += deltaVel;
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
                                Ogre::Vector2 inputVec = Ogre::Vector2(0, 0);
                                GetWASDVector(input.ptr, inputVec);
                                float speed = ctr.ControllSpeed;
                                if (input.ptr->GetInputState().test(eIC_Faster))
                                    speed *= 2;
                                Ogre::Vector3 deltaVel = speed * Ogre::Vector3(inputVec.x, 0, -inputVec.y) * e.delta_time();
                                Ogre::Vector2 pressedDeltaMouse = input.ptr->DeltaDownMousePos() * input.ptr->GetMouseSensitivity();
                                pressedDeltaMouse *= e.delta_time();

                                Ogre::Radian offset = Ogre::Radian(pressedDeltaMouse.y);
                                Ogre::Radian newVal = cam.ptr->getRealOrientation().getPitch() + offset;
                                
                                Ogre::Radian clamped = std::clamp(newVal, -Ogre::Radian(M_PI / 2), Ogre::Radian(M_PI / 2));
                                offset = clamped - cam.ptr->getRealOrientation().getPitch();

                                rendEngine.ptr->GetRT()->RC_LambdaAction([=] { 
                                        cam.ptr->moveRelative(deltaVel);
                                        cam.ptr->yaw(Ogre::Radian(-pressedDeltaMouse.x));
                                        cam.ptr->pitch(offset);
                                    });
                            });
                    });
            });
}


void GetWASDVector(InputHandler* input, Ogre::Vector2 &vec)
{
    if (input->GetInputState().test(eIC_GoLeft))
        vec -= Ogre::Vector2(1, 0);
    if (input->GetInputState().test(eIC_GoRight))
        vec += Ogre::Vector2(1, 0);
    if (input->GetInputState().test(eIC_GoDown))
        vec -= Ogre::Vector2(0, 1);
    if (input->GetInputState().test(eIC_GoUp))
        vec += Ogre::Vector2(0, 1);
}