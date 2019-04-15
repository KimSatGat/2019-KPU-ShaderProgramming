#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_OriXY;
in float v_Radius;

void main()
{
	vec4 newColor;
	float dis = sqrt(v_OriXY.x*v_OriXY.x + v_OriXY.y*v_OriXY.y);
	if(dis < v_Radius)
	{
		newColor = v_Color;
		newColor.a = pow(1 - dis / v_Radius, 7);
	}
	else
	{
		newColor = vec4(0, 0, 0, 0);
	}
	FragColor = newColor;
}
