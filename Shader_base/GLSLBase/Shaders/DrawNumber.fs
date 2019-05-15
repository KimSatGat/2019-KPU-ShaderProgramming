#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D u_Texture;

uniform int u_Number[3];	// 3 digit

void main()
{			
// 세자리 숫자 표현
	vec2 newTex = v_Tex;		// PNG 경우 반전 시켜야하므로 newTex = vec2(v_Tex.x, 1.0 - v_Tex.y);

	// x_tex coord
	newTex.x = fract(v_Tex.x * 3.0);
	
	// cal index
	int newIndex = int(floor(v_Tex.x * 3.0));
	
	float newy = 9.0 - float(u_Number[newIndex]);
	newTex.y = newTex.y / 10.0 + newy/10.0;
	vec4 newColor = texture(u_Texture, newTex);	
	FragColor = newColor;
}
