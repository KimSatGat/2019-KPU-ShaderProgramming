#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Temp;

uniform float u_Time;
uniform vec2 u_Points[5];

void main()
{
	vec2 newUV = v_Temp.xy - vec2(0.5, 0.5); //-0.5~0.5, x, y

	float pointGrey = 0;
	
	float distance = length(newUV);
	float newTime = fract(u_Time);
	float ringWidth = 0.1;

	if(distance < newTime + ringWidth && distance > newTime)
	{
		float temp = (distance - newTime) / ringWidth;
		pointGrey = temp;

		for(int i=0; i<5; i++)
		{
			vec2 newPoint = u_Points[i];
			vec2 newVec = newPoint - newUV;
			float distance = length(newVec);
			if(distance < 0.1)
			{
				pointGrey += 0.5 * pow((1-(distance/0.1)), 5);
			}
		}
	}	
	FragColor = vec4(pointGrey);
}
