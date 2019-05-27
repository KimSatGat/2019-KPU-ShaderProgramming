#version 450

layout(location = 0) in vec3 a_Position;

const float PI = 3.141592;
uniform float u_Time;
out vec3 color;
void main()
{
	vec3 newPos = a_Position;	

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
}
