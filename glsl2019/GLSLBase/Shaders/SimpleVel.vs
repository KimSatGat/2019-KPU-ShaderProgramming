#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_StartLife;

uniform float u_Time;
uniform bool u_Repeat = true;		//True일땐 반복 false일땐 다른곳에 그리기

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

		float remainingLife = lifeTime - life;

		if(u_Repeat)
		{
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}

		if(remainingLife > 0)
		{
			// float newTime = fract(u_Time);	// 0 ~ 1 repeat
			newPos += (a_Vel * newTime) + (0.5f * c_Gravity * newTime * newTime);	// 중력 가속도 적용
		}
		else
		{
			newPos = vec3(10000,10000,10000);
		}
	}

	else 
	{
		newPos = vec3(10000,10000,10000);
	}

	gl_Position = vec4(newPos.xyz, 1);	
}
