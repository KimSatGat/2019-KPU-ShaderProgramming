#version 450

layout(location = 0) in vec3 a_Position;

const float PI = 3.141592;
uniform float u_Time;
uniform vec2 u_Points[5];
out vec3 color;
int shape = 0;

void Flag()
{
	vec3 newPos = a_Position;	

	/* 깃발 코드 주석
	// 0 ~ 1
	float additionalValueX = newPos.x + 0.5;
	float additionalValueY = newPos.y + 0.5;

	float periodX = 1.0 + (1.0 - additionalValueY) * 0.5;
	float periodY = 1.0 + additionalValueX * 0.5;

	// x :: -0.5 ~ 0.5  --> +0.5 -> 0 ~ 1 --> * 2 * PT -> 0 ~ 2PI
	float valueX = (additionalValueY * 2 * PI * periodX) - u_Time;
	float valueY = (additionalValueX * 2 * PI * periodY) - u_Time;	

	float sinValueX = sin(valueX) * 0.1;
	float sinValueY = sin(valueY) * 0.2;
	
	// x
	newPos.x = newPos.x - sinValueX * additionalValueX;

	// y :: -0.5 ~ 0.5
	newPos.y = newPos.y + sinValueY * additionalValueX;
	
	color = vec3(sinValueY + 0.5f, sinValueY + 0.5f, sinValueY + 0.5f);

	gl_Position = vec4(newPos, 1);
	*/
}

void Wave()
{
	float grey = 0;
	vec3 newPos = a_Position.xyz;

	for(int i = 0; i<5; i++)
	{
		vec2 target;
		vec2 source;
		target = a_Position.xy;
		source = u_Points[i];
		float dis = length(target - source) * 4 * PI;	// 0 ~ 0.5 --> 0 ~ 4PI
		grey += sin(dis - u_Time);
	}
	newPos.z += grey;

	gl_Position = vec4(newPos.xyz, 1);

	float v_Grey = (grey + 1.0) / 2.0;
	color = vec3(v_Grey);
}


void SphereMapping()
{			
		float r = 0.5;
		float beta = (a_Position.x + 0.5) * 2 * PI;
		float theta = (a_Position.y + 0.5) * PI;
		vec3 spherePos = a_Position.xyz;

		spherePos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(beta)
		);

		vec3 originPos = a_Position.xyz;

		float interpol = fract(u_Time);

		vec3 newPos = mix(originPos, spherePos, interpol);

		gl_Position = vec4(newPos.xyz, 1);
		color = vec3(1);	
}
void main()
{
	// Flag();
	// Wave();	
	SphereMapping();
}
