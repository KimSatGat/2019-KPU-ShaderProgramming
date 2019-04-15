#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_StartLife;

uniform float u_Time; //0.....
uniform bool u_Repeat = true;

const vec3 c_Gravity = vec3(0, -5, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;

	float newTime = u_Time - startTime; 

	if(newTime > 0)
	{
		float life = newTime;
		
		float remainingLife = lifeTime-life;

		if(u_Repeat == true)
		{
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}

		if(remainingLife < 0)
		{
			newPos = vec3(100000, 100000, 100000);
		}
		else
		{
			newPos += a_Vel * newTime + 0.5*c_Gravity*newTime*newTime;
		}
	}
	else
	{
		newPos = vec3(100000, 100000, 100000);
	}

	gl_Position = vec4(newPos, 1);
}






































/*
in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_StartLife;

uniform float u_Time; //0.....
uniform bool u_Repeat = true; //0.....

const vec3 c_Gravity = vec3(0, -10, 0);

void main()
{
	vec3 newPos = a_Position.xyz;

	float newTime = u_Time - a_StartLife.x;

	if(newTime > 0)
	{
		float remainingTime = a_StartLife.y - newTime;
		if(u_Repeat)
		{
			remainingTime = 1.f;
			//newTime = fract(newTime/a_StartLife.y) * a_StartLife.y;
			newTime = mod(newTime, a_StartLife.y);
		}
		if(remainingTime > 0)
		{
			newPos += a_Vel * newTime + 0.5*c_Gravity*newTime*newTime;
		}
		else
		{
			newPos = vec3(10000, 10000, 10000);
		}
	}
	else
	{
		newPos = vec3(10000, 10000, 10000);
	}

	gl_Position = vec4(newPos, 1);
}*/
