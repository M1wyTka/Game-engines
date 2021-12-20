#include "ECS/ScriptECS.h"
#include "ECS/GeneralECS.h"
#include "ECS/KinematicsECS.h"
#include "ECS/ControlECS.h"
#include "OgreLogManager.h"

void LoadScriptSystems(flecs::world& world) 
{
	static auto scriptSystemQuery = world.query<ScriptSystemPtr>();
	static auto inputQuery = world.query<InputHandlerPtr>();

	world.system<ScriptComponent, const DeltaKinematics>()
		.kind(flecs::PreUpdate)
		.each([&](flecs::entity e, ScriptComponent& scriptNode, const DeltaKinematics& dkin)
			{
				scriptNode.ptr->PreUpdate();
			});

	world.system<ScriptComponent, const Kinematics, const DeltaKinematics>()
		.kind(flecs::OnUpdate)
		.each([&](flecs::entity e, ScriptComponent& scriptNode, const Kinematics& kin, const DeltaKinematics& dkin)
			{
				scriptNode.ptr->Update(e.delta_time());
			});


	world.system<const Controllable, ScriptComponent, Kinematics, DeltaKinematics>()
		.kind(flecs::OnValidate)
		.each([&](flecs::entity e, const Controllable&, ScriptComponent& script, Kinematics& t, DeltaKinematics& dkin)
			{
				dkin.DeltaPos = script.ptr->GetDeltaKinematics().DeltaPos;	
			});
}