#version 450

in vec3 a_Position;
in vec2 a_Temp;
in vec2 a_StartLife;

uniform float u_Time;

const float PI = 3.141592;

void main()
{
	vec3 newPos = a_Position.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;

	//float newTime = fract(u_Time);

	float newTime = u_Time - startTime;

	if(newTime > 0)
	{
		float life = newTime;

		float remainingLife = lifeTime = life;

		if(remainingLife > 0)
		{
			newPos.x += newTime * a_Temp.x;
			newPos.y += sin(newTime * PI * 2) * a_Temp.y;

			//newPos.x += sin(u_Time * a_Temp.x);			
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