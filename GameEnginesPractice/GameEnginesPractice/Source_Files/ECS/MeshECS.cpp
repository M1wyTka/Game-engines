#include "ECS/MeshECS.h"
#include "function2/function2.hpp"
#include "ECS/KinematicsECS.h"

void LoadMeshSystems(flecs::world& world)
{
    static auto renderQuery = world.query<RenderEnginePtr>();
    LoadMeshCreationSystem(world, renderQuery);
    LoadMeshPositionsUpdateSystem(world, renderQuery);
}

void LoadMeshCreationSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery)
{
    world.system<Kinematics, SceneObj>()
        .kind(flecs::PreUpdate)
        .each([&](flecs::entity e, Kinematics& kins, SceneObj& scnObj)
            {
                kins.Rotation = scnObj.pSceneObject->GetOrientation();
                kins.Position = scnObj.pSceneObject->GetPosition();
                kins.Scale = scnObj.pSceneObject->GetScale();
            });

    world.system<const SceneObjectPattern, const Kinematics>()
        .kind(flecs::OnLoad)
        .each([&](flecs::entity e, const SceneObjectPattern& stencil, const Kinematics& kins)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        if (rendEngine.ptr->IsInitialized())
                            rendEngine.ptr->GetRT()->RC_LambdaAction([=] {
                                SceneObject* temp = rendEngine.ptr->RT_CreateSceneObject(stencil.name, stencil.meshName);
                                temp->SetVisibility(stencil.isVisible);
                                temp->SetPosition(kins.Position);
                                //temp->Rotate(kins.Rotation);
                                temp->SetScale(kins.Scale);
                                e.set<SceneObj>(SceneObj{ temp });
                                e.remove<SceneObjectPattern>();
                            });
                    });
            });
}

void LoadMeshPositionsUpdateSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery)
{
    world.system<SceneObj, const Kinematics, const DeltaKinematics>()
        .kind(flecs::OnStore)
        .iter([&](flecs::iter& it, SceneObj* sceneNodes, const Kinematics* kins, const DeltaKinematics* dKins)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        rendEngine.ptr->GetRT()->RC_LambdaAction([=] {
                            for (int i : it)
                            {
                                sceneNodes[i].pSceneObject->Translate(dKins[i].DeltaPos);
                                sceneNodes[i].pSceneObject->SetOrientation(dKins[i].DeltaRot);
                                //sceneNodes[i].pSceneObject->SetScale(dKins[i].DeltaScale);
                            }
                        });
                    });
            });
}