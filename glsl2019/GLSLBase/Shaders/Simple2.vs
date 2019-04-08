#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp;
in float a_RandV;

uniform float u_Time;

const float PI = 3.141592;
const mat3 c_RP = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);
const vec3 c_Gravity = vec3(0, -3, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	vec3 newVel = a_Vel.xyz;

	float start = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w;

	//float newTime = fract(u_Time);

	float newTime = u_Time - start;

	if(newTime > 0)
	{				
		newPos.x += sin(aValue * PI);
		newPos.y += cos(aValue * PI);
		newTime = mod(newTime, lifeTime);			

		newVel = newVel + c_Gravity * newTime;	// 속도

		newPos = newPos + a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime;	// 위치
			
		vec3 vSin = a_Vel * c_RP;
		newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp;
		
		// newPos.x += newTime * a_Vel.x;
		// newPos.y += sin(newTime * PI * 2) * (u_Time * newTime);
		// newPos.y += sin(newTime * PI * 2) * a_Vel.y;
		// newPos.x += sin(u_Time * a_Vel.x);							
	}
	else
	{
		newPos = vec3(10000,10000,10000);
	}

	gl_Position = vec4(newPos.xyz, 1);
}