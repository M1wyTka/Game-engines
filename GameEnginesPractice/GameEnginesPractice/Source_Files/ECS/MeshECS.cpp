#include "ECS/MeshECS.h"

void LoadMeshSystems(flecs::world& world)
{
    static auto renderQuery = world.query<RenderEnginePtr>();
    LoadMeshCreationSystem(world, renderQuery);
    LoadMeshDeliverySystem(world, renderQuery);
    LoadMeshPositionsUpdateSystem(world, renderQuery);
}

void LoadMeshCreationSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery)
{
    world.system<const MeshName>()
        .each([&](flecs::entity e, const MeshName& nm)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        if (rendEngine.ptr->IsInitialized())
                        {
                            uint32_t index = rendEngine.ptr->GetRT()->RC_CreateSceneObject(nm.name);
                            e.set(SceneNodeDeliveryIndex{ index });
                            e.remove<MeshName>();
                        }
                    });
            });
}

void LoadMeshDeliverySystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery)
{
    world.system<const SceneNodeDeliveryIndex, const Position>()
        .each([&](flecs::entity e, const SceneNodeDeliveryIndex& delivery_index, const Position& pos)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        if (rendEngine.ptr->IsInitialized())
                        {
                            std::map<uint32_t, void*> delivery = rendEngine.ptr->GetRT()->GetDeliveryQueue();
                            auto fiter = delivery.find(delivery_index.idx);
                            if (fiter == delivery.end())
                                return;
                            SceneObject* pSceneNode = (SceneObject*)fiter->second;
                            rendEngine.ptr->GetRT()->RC_UpdateActorPosition(pSceneNode, pos);

                            e.set<SceneObj>(SceneObj{ pSceneNode });
                            e.remove<SceneNodeDeliveryIndex>();
                        }
                    });
            });

    world.system<const SceneObj, const Scale>()
        .each([&](flecs::entity e, const SceneObj& node, const Scale& sc)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        rendEngine.ptr->GetRT()->RC_UpdateActorScale(node.pSceneObject, sc);
                        e.remove<Scale>();
                    });
            });
}

void LoadMeshPositionsUpdateSystem(flecs::world& world, flecs::query<RenderEnginePtr>& renderQuery)
{
    world.system<SceneObj, const Position>()
        .each([&](SceneObj& scene_node, const Position& pos)
            {
                renderQuery.each([&](RenderEnginePtr rendEngine)
                    {
                        rendEngine.ptr->GetRT()->RC_UpdateActorPosition(scene_node.pSceneObject, pos);
                    });
            });
}