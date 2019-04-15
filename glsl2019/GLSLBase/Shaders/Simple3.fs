#version 450

layout(location=0) out vec4 FragColor;
in vec4 v_Temp;

uniform vec2 u_Points[5];

const vec2 c_Points[2] = {vec2(0,0), vec2(0.1,0.1)};

void main()
{
	
	vec2 newUV = v_Temp.xy - vec2(0.5,0.5); // -0.5 ~ 0.5
	float pointGrey = 0;
	for(int i = 0; i < 2; i++)
	{
		vec2 newPoint = c_Points[i];
		vec2 newVector = newPoint - newUV;
		float distance = sqrt(newVector.x * newVector.x + newVector.y * newVector.y);
		if(distance < 1)
		{
			pointGrey += 0.8;
		}
	}

	//float distance = sqrt(v_Temp.x * v_Temp.x + v_Temp.y * v_Temp.y);
	//float grey = sin(distance * 3.141592 * 4 * 5);	

	/*
	if(distance < 0.5)
	{
		FragColor = vec4(sin(distance * 3.14));
	}
	else
	{	
		FragColor = vec4(0);	// 화면의 사각형 color	
	}
	*/
	FragColor = vec4(pointGrey);
}
