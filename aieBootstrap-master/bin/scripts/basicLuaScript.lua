AIState = 0;
timer = 0;

function Initialise();
end;

function Destroy();
end;

function Update(delta_time);
	local x, y = GetPosition()
	local ballX, ballY = GetBallPosition()
	local dirX = ballX - x;
	local dirY = ballY - y;
	local mag = math.sqrt((dirX*dirX) + (dirY*dirY))
	local nX = dirX / mag;
	local nY = dirY / mag;
	local targetAngle = math.atan(nY, nX);
	local angle = GetAngle()
	local rot = targetAngle - angle;
	
	if AIState == 0 then
		ApplyForce(nY * 500, nX * 500)
		ApplyRotation(rot)
	elseif AIState == 1 then
		timer = timer + delta_time
		if timer > 1.5 then
			AIState = 0
	end
	
	ApplyForce(nY*-500, nX*-500)
	ApplyRotation(rot)
	end
end;

function Draw();
end;

function OnCollision(otherX, otherY);
	AIState = 1;
	timer = 0;
end;