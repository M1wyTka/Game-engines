#include "ECS/ScriptECS.h"
#include "ECS/GeneralECS.h"
#include "ECS/KinematicsECS.h"

void LoadScriptSystems(flecs::world& world) 
{
	static auto scriptSystemQuery = world.query<ScriptSystemPtr>();

	world.system<ScriptComponent, const Kinematics>()
		.each([&](flecs::entity e, ScriptComponent& scriptNode, const Kinematics& kin)
			{
				scriptNode.ptr->Update(e.delta_time());
			});
}