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
    world.system<const MeshName, const Position, const Rotation, const Scale>().kind(flecs::PreUpdate)
        .each([&](flecs::entity e, const MeshName& nm, const Position& pos, const Rotation& rot, const Scale& sc)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        if (rendEngine.ptr->IsInitialized())
                            rendEngine.ptr->GetRT()->RC_LambdaAction([=] {
                                SceneObject* temp = rendEngine.ptr->RT_CreateSceneObject(nm.name, nm.name);
                                temp->SetPosition(pos.val);
                                temp->Rotate(rot.val);
                                temp->SetScale(sc.val);
                                e.set<SceneObj>(SceneObj{ temp });
                                e.remove<MeshName>();
                            });
                    });
            });
}

void LoadMeshPositionsUpdateSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery)
{
    world.system<SceneObj, const Position, const DeltaPos>()
        .kind(flecs::OnStore)
        .iter([&](flecs::iter& it, SceneObj* sceneNodes, const Position* pos, const DeltaPos* delta)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {   
                        rendEngine.ptr->GetRT()->RC_LambdaAction([=] {
                            for (int i : it)
                            {
                                sceneNodes[i].pSceneObject->Translate(delta[i].val);
                            }
                        });
                    });
            });

    world.system<SceneObj, const DeltaRotation>()
        .kind(flecs::PreStore)
        .iter([&](flecs::iter& it, SceneObj* sceneNodes, const DeltaRotation* delta)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        rendEngine.ptr->GetRT()->RC_LambdaAction([=] {
                                for (int i : it)
                                {
                                    sceneNodes[i].pSceneObject->SetOrientation(delta[i].val);
                                }
                            });
                    });
            });

    world.system<SceneObj, const DeltaScale>()
        .kind(flecs::PreStore)
        .iter([&](flecs::iter& it, SceneObj* sceneNodes, const DeltaScale* delta)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        rendEngine.ptr->GetRT()->RC_LambdaAction([=] {
                                for (int i : it)
                                {
                                    sceneNodes[i].pSceneObject->SetScale(delta[i].val);
                                }
                            });
                    });
            });
}