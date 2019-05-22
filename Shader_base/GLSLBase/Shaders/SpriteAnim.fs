#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D u_Texture;

uniform float u_Number;
uniform float u_Resolx;
uniform float u_Resoly;

void main()
{		
	vec2 newTex = v_Tex;		// PNG 경우 반전 시켜야하므로 newTex = vec2(v_Tex.x, 1.0 - v_Tex.y);

	float x_index = floor(fract(u_Number / u_Resolx) * u_Resolx);
	float y_index = floor(floor(u_Number / u_Resolx));	// floor는 몫

	// tex coord
	newTex.x = v_Tex.x / u_Resolx + x_index / float(u_Resolx);
	newTex.y = v_Tex.y / u_Resoly + y_index / float(u_Resoly);
	
	
	vec4 newColor = texture(u_Texture, newTex);	
	FragColor = newColor;
}
