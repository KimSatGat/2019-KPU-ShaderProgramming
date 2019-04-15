#version 450

//Attributes
in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp;
in float a_RandV;
in vec4 a_Color;

//Varying --> fragment shader input
out vec4 v_Color;
out vec2 v_OriXY;
out float v_Radius;

uniform float u_Time;

const float PI = 3.141592;
const mat3 c_RP = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);
const vec3 c_Gravity = vec3(0, -1, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	v_OriXY = a_Position.xy;
	v_Radius = abs(a_Position.x);
	vec3 newVel = a_Vel.xyz;

	float start = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w;

	float newTime = u_Time - start;

	float newAlpha = 1;

	if(newTime > 0)
	{
		newPos.x += sin(a_RandV * 2*PI);
		newPos.y += cos(a_RandV * 2*PI);

		newTime = mod(newTime, lifeTime);

		newVel = newVel + c_Gravity*newTime;

		newPos = newPos + a_Vel*newTime + 0.5*c_Gravity*newTime*newTime;

		vec3 vSin = a_Vel*c_RP;
		
		newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp;

		newAlpha = pow(1 - newTime/lifeTime, 5);
	}
	else
	{
		newPos = vec3(100000, 100000, 100000);
	}
	
	gl_Position = vec4(newPos.xyz, 1);

	v_Color = a_Color;
	v_Color.a = newAlpha;
}





























/*

void Renderer::GenQuadsVBO(int count, bool bRandPos, GLuint * id, GLuint * vCount)
{
	float size = 0.005f;
	int countQuad = count;
	int verticesPerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1; //x,y,z, vx,vy,vz, s,l, r, a, randValue
	int arraySize = countQuad * verticesPerQuad * floatsPerVertex;
	float *vertices = new float[arraySize];

	for (int i = 0; i < countQuad; i++)
	{
		float randX, randY;
		float randVelX, randVelY, randVelZ;
		float startTime, lifeTime;
		float startTimeMax = 6.f;
		float lifeTimeMax = 3.f;
		float ratio, amp;
		float ratioMin = 0.5f;
		float ratioThres = 1.f;
		float ampMin = -0.05f;
		float ampThres = 0.1f;
		float randValue;
		float randValueMin = 0.f, randValueThres = 1.f;

		int index = i * verticesPerQuad * floatsPerVertex;

		if (bRandPos)
		{
			randX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
			randY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else
		{
			randX = 0.f;
			randY = 0.f;
		}

		randVelX = 0.2f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelY = 0.2f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelZ = 0.f;

		startTime = ((float)rand() / (float)RAND_MAX) * startTimeMax;
		lifeTime = ((float)rand() / (float)RAND_MAX) * lifeTimeMax;

		ratio = ratioMin + ((float)rand() / (float)RAND_MAX) * ratioThres;
		amp = ampMin + ((float)rand() / (float)RAND_MAX) * ampThres;

		randValue = randValueMin + ((float)rand() / (float)RAND_MAX) * randValueThres;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;
	}

	GLuint vboID = 0;

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*arraySize, vertices, GL_STATIC_DRAW);
	*vCount = countQuad * verticesPerQuad;
	*id = vboID;
}



void Renderer::Lecture7(float dirX, float dirY)
{
	// TODO: 여기에 구현 코드 추가.
	GLuint shader = m_SinTrailShader;

	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");

	glUniform1f(uTime, g_Time);
	g_Time += 0.01;

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aVel = glGetAttribLocation(shader, "a_Vel");
	GLuint aSLRA = glGetAttribLocation(shader, "a_StartLifeRatioAmp");
	GLuint aRand = glGetAttribLocation(shader, "a_RandValue");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aSLRA);
	glEnableVertexAttribArray(aRand);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads1);

	//(0x, 1y, 2z, 3vx, 4vy, 5vz, 6s, 7l,  8x, 9y, 10z, 11vx, 12vy, 13vz, 14s, 15l, )
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11,
		(GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aSLRA, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 11,
		(GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aRand, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 11,
		(GLvoid*)(sizeof(float) * 10));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount1);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aSLRA);
	glDisableVertexAttribArray(aRand);
}

#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp;
in float a_RandValue;;

uniform float u_Time;
uniform vec3 u_ExtAcc;

const float PI = 3.141592;

const mat3 c_Rot = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);
const vec3 c_Gravity = vec3(0, -2, 0);

void main()
{
	vec3 newPos = a_Position.xyz;
	vec3 newVel = a_Vel.xyz;

	float start = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w;

	float newTime = u_Time - start;

	if(newTime > 0)
	{
		newPos.x += sin(a_RandValue*2*PI);
		newPos.y += cos(a_RandValue*2*PI);

		newTime = mod(newTime, lifeTime) ;
		newVel += newTime * (c_Gravity + u_ExtAcc);
		newPos += newTime * newVel;
		vec3 rotVel = newVel*c_Rot;
		//newPos.x += newTime;
		newPos += rotVel*sin(newTime * 2 * PI * ratio) * amp;
	}
	else
	{
		newPos = vec3(100000, 100000, 100000);
	}
	
	gl_Position = vec4(newPos.xyz, 1);
}*/