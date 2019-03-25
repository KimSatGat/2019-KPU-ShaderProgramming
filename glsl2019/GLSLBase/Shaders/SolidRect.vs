#version 450

in vec4 a_Position;
in vec4 a_Color;

uniform float u_Time;	
uniform float u_Theta;

void main()
{

	float newX, newY;

	newX = cos(u_Theta * 3.141592);
	newY = sin(u_Theta * 3.141592);

	newX += a_Position.x;
	newY += a_Position.y;

	gl_Position = vec4(newX,newY,0, 1);
}
