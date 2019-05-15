#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D u_TexSampler;	// 0번에 바인드되어있는 텍스처를 샘플링하여 가져옴
uniform float u_Time;

void main()
{		
	//vec2 newTex = fract(v_Tex * 4 + vec2(cos(u_Time), u_Time));
	//vec4 newColor = vec4(newTex, 0, 1);

	vec2 newTex = abs(v_Tex - vec2(0, 0.5)) + vec2(0, 0.5);

	vec4 newColor = texture(u_TexSampler, newTex);
		

	FragColor = newColor;

	// v_Tex : x 0~1 -> 0~2PI
	//float x = v_Tex.x * 2 * 3.14 + u_Time;
	//float sinResult = (sin(x) + 1) / 2;

	//if(sinResult > v_Tex.y && sinResult - 0.01 < v_Tex.y)
	//{
    //		newColor = vec4(1);
	//}
	
}
