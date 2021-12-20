Entity = {
    Properties = {
        Controllable = 1,
        HasPhysics = 0,
		IsStatic = 0,
    },
    up_vector = Vector3(0.0, 1.0, 0.0)
}

Entity.OnInit = function()
	Kinematics.Rotation = Quaternion(Radian(0), Entity.up_vector);
	DeltaKinematics.Rotation = Quaternion(Radian(0), Entity.up_vector);
	DeltaKinematics.Position = Vector3(0.0, 0.0, 0.0)
end

Entity.OnUpdate = function(dt)
	local deltaMoveVelocity = 0.0;
    local deltaRotationVelocity = 0.0;
    
    if (inputHandler:isCommandActive(2)) then
        deltaMoveVelocity = deltaMoveVelocity + Entity.Parameters.move_speed;
    end
    if (inputHandler:isCommandActive(3)) then
        deltaMoveVelocity = deltaMoveVelocity - Entity.Parameters.move_speed;
    end
	deltaRotationVelocity = deltaRotationVelocity * dt;
    Kinematics.Position = Vector3(0.0, 0.01, 0.0);
	DeltaKinematics.Position = Vector3(0.0, 0.01, 0.0);
end

Entity.GetPosition = function()
    return Kinematics.Position;
end

Entity.GetOrientation = function()
    return Entity.Rotation;
end

Entity.GetPosition = function()
    return Kinematics.Position;
end