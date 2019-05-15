#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform sampler2D u_TexSampler1;
uniform sampler2D u_TexSampler2;
uniform sampler2D u_TexSampler3;
uniform sampler2D u_TexSampler4;

void main()
{		
	vec4 newColor;
	vec2 newTex = v_Tex;		// PNG 경우 반전 시켜야하므로 newTex = vec2(v_Tex.x, 1.0 - v_Tex.y);
	
	if(newTex.x < 0.5 && newTex.y > 0.5)
	{
		newColor = texture(u_TexSampler1, vec2(newTex.x * 2, fract(newTex.y * 2)));
	}
	
	else if(newTex.x > 0.5 && newTex.y > 0.5)
	{
		newColor = texture(u_TexSampler2, vec2(fract(newTex.x * 2), fract(newTex.y * 2)));
	}

	else if(newTex.x < 0.5 && newTex.y < 0.5)
	{
		newColor = texture(u_TexSampler3, vec2(newTex.x * 2, newTex.y * 2));
	}

	else
	{
		newColor = texture(u_TexSampler4, vec2(fract(newTex.x * 2), newTex.y * 2));
	}

	FragColor = vec4(newColor);	
}
