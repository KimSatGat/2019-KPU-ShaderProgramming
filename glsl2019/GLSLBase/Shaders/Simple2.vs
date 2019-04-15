#version 450

// Attributes
in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp;
in float a_RandVal;
in vec4 a_Color;

// Varying --> fragment shader input
out vec4 v_color;
out vec2 v_OriXY;
out float v_Radius;
uniform float u_Time;

const float PI = 3.141592;
const mat3 c_RP = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);
const vec3 c_Gravity = vec3(0, -3, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	v_OriXY = a_Position.xy;
	v_Radius = abs(a_Position.x);
	vec3 newVel = a_Vel.xyz;

	float start = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w;
	
	float newTime = u_Time - start;

	float newAlpha = 1;

	if(newTime > 0)
	{
		newPos.x += sin(a_RandVal * PI);
		newPos.y += cos(a_RandVal * PI);
		newPos.z = 0;
		
		newTime = mod(newTime, lifeTime);

		newVel = newVel + c_Gravity * newTime;	// 속도

		newPos = newPos + a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime;	// 위치
			
		vec3 vSin = a_Vel * c_RP;

		newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp;		
		
		newAlpha = 1 - newTime / lifeTime;
	}
	else
	{
		newPos = vec3(10000,10000,10000);
	}

	gl_Position = vec4(newPos.xyz, 1);

	v_color = a_Color;
	
	v_color.a = newAlpha;
}