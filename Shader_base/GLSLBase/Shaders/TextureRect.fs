#version 450

out vec4 FragColor;

in vec2 v_Tex;

uniform float u_Time;

void main()
{	
	vec4 newColor = vec4(0);
	// v_Tex : x 0~1 -> 0~2PI
	float x = v_Tex.x * 2 * 3.14 + u_Time;
	float sinResult = (sin(x) + 1) / 2;

	if(sinResult > v_Tex.y && sinResult - 0.01 < v_Tex.y)
	{
		newColor = vec4(1);
	}

	FragColor = newColor;
}
