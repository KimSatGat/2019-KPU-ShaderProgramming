#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>



Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	//m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	//m_SimpleVelShader = CompileShaders("./Shaders/SimpleVel.vs", "./Shaders/SimpleVel.fs");
	m_Simple2Shader = CompileShaders("./Shaders/Simple2.vs", "./Shaders/Simple2.fs");
	m_Simple3Shader = CompileShaders("./Shaders/Simple3.vs", "./Shaders/Simple3.fs");
	//Create VBOs
	CreateVertexBufferObjects();
}

void Renderer::CreateVertexBufferObjects()
{
	/*
	float size = 0.02f;
	float rect[]
		=
	{
		-size, -size, 0.f, 0.5,	// x, y, z, value
		-size, size, 0.f, 0.5,
		size, size, 0.f, 0.5, //Triangle1 버텍스 3개
		-size, -size, 0.f, 0.5,
		size, size, 0.f, 0.5,
		size, -size, 0.f, 0.5 //Triangle2 버텍스 3개
	};

	glGenBuffers(1, &m_VBORect);			// id를 지정
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);	// GL_ARRAY_BUFFER 라는 작업대로 이동
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);		// 작업대에서 내가 원하는 데이터를 넘겨준다 -> VBORect에 내가 원하는 정보가 있게된다

	float color[]
		=
	{
		1, 0, 0.f, 1,	// r, g, b, a
		1, 0, 0.f, 1,
		1, 0, 0.f, 1, 
		1, 0, 0.f, 1,
		1, 0, 0.f, 1,
		1, 0, 0.f, 1
	};

	glGenBuffers(1, &m_VBORectColor);			// id를 지정
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);	// GL_ARRAY_BUFFER 라는 작업대로 이동
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);		// 작업대에서 내가 원하는 데이터를 넘겨준다 -> VBORect에 내가 원하는 정보가 있게된다

	//lecture2
	
	float triangleVertex[]
		=
	{
		-1.0, 0.f, 0.f, 0.f, 1.0, 0.f, 1.0, 0.f, 0.f, // 9 floats		
	};

	glGenBuffers(1, &m_VBOLecture2);	// 개수 하나, 이름 지정 
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertex), triangleVertex, GL_STATIC_DRAW);
	*/

	//GenQuads(1000);
	//GenQuads2(1000);
	GenQuads3(1);
	//CreateGridMesh();
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

float offsetTime = 0;
float offsetTheta = 0;
void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);	// m_SolidRectShader 쉐이더를 쓰겠다.

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	GLuint uTheta = glGetUniformLocation(m_SolidRectShader, "u_Theta");
	
	offsetTheta += 0.001f;
	offsetTime += 0.0001f;
	
	glUniform1f(uTheta, offsetTheta);
	//glUniform1f(uTime, offsetTime);			//glUniform1f float값 하나 넣겠다 uTime에 의해 u_Time이 매프레임 마다 offset값이 됨.

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");	
	int attribColor = glGetAttribLocation(m_SolidRectShader, "a_Color");

	glEnableVertexAttribArray(attribPosition);	// 0 번으로 로케이션이 있는 어트리뷰트를 enable 하겠다. SolidRect.vs의 a_Position이 사용가능하게 됨.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 4 /*세개씩 끊어서 하나의 버텍스를 구성해라!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 4 /*다음 위치를 알려주는곳 (영어로는 stride)*/, 0);
	// 3개씩 읽은 float 값을 SolidRect.vs의 a_Position에 값이 전달 된다.

	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(attribColor, 4 /*세개씩 끊어서 하나의 버텍스를 구성해라!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 4 /*다음 위치를 알려주는곳 (영어로는 stride)*/, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribColor);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);		// 나중에 설명함  -> 0번지를 enable
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(0, 3 /*세개씩 끊어서 하나의 버텍스를 구성해라!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 3 /*다음 위치를 알려주는곳 (영어로는 stride)*/, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);	// 트라이앵글 형태, , 버텍스 3개

	glDisableVertexAttribArray(0);	// 0번지 disable
}

// 버텍스 생성
void Renderer::GenQuads(int n)
{
	float offset = 0.01f;	// 반지름
	float centerX, centerY;
	int count = 0;
	
	m_num = n * 6;					// 버텍스 개수
	m_size = n * 8 * 6;				// 원소 개수	8 == 버텍스의 원소 개수 6 == 쿼드당 버텍스 개수
	m_Array = new float[m_size];	// 배열 생성	

	srand(GetTickCount());

	for (int i = 0; i < n; i++)
	{		
		float ranVelx, ranVely, ranVelz;
		float startTime, lifeTime;
		float startTimeMax = 6.f;
		float lifeTimeMax = 1000.f;

		centerX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);	// 중심점 생성 -1 ~ 1
		centerY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);	// 중심점 생성 -1 ~ 1		

		ranVelx = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		ranVely = ((float)rand() / (float)RAND_MAX) * 0.00001f;
		ranVelz = 0.f;

		startTime = ((float)rand() / (float)RAND_MAX) * startTimeMax;
		lifeTime = ((float)rand() / (float)RAND_MAX) * lifeTimeMax;

		// Ver1
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = 0;					// z
		
		// Ver1의 방향벡터   --> 같은 도형이므로 방향벡터를 같게 넣어야한다.
		m_Array[count++] = ranVelx;	// x
		m_Array[count++] = ranVely;	// y
		m_Array[count++] = 0;	// z

		// 생성시간, 생존시간
		m_Array[count++] = startTime;
		m_Array[count++] = lifeTime;

		// Ver2
		m_Array[count++] = centerX + offset;
		m_Array[count++] = centerY + offset;
		m_Array[count++] = 0;

		// Ver1의 방향벡터
		m_Array[count++] = ranVelx;	// x
		m_Array[count++] = ranVely;	// y
		m_Array[count++] = 0;	// z

		// 생성시간, 생존시간
		m_Array[count++] = startTime;
		m_Array[count++] = lifeTime;

		// Ver3
		m_Array[count++] = centerX - offset;
		m_Array[count++] = centerY - offset;
		m_Array[count++] = 0;

		// Ver1의 방향벡터
		m_Array[count++] = ranVelx;	// x
		m_Array[count++] = ranVely;	// y
		m_Array[count++] = 0;	// z

		// 생성시간, 생존시간
		m_Array[count++] = startTime;
		m_Array[count++] = lifeTime;

		// Ver4
		m_Array[count++] = centerX + offset;
		m_Array[count++] = centerY + offset;
		m_Array[count++] = 0;

		// Ver1의 방향벡터
		m_Array[count++] = ranVelx;	// x
		m_Array[count++] = ranVely;	// y
		m_Array[count++] = 0;	// z

		// 생성시간, 생존시간
		m_Array[count++] = startTime;
		m_Array[count++] = lifeTime;

		// Ver5
		m_Array[count++] = centerX - offset;
		m_Array[count++] = centerY - offset;
		m_Array[count++] = 0;

		// Ver1의 방향벡터
		m_Array[count++] = ranVelx;	// x
		m_Array[count++] = ranVely;	// y
		m_Array[count++] = 0;	// z

		// 생성시간, 생존시간
		m_Array[count++] = startTime;
		m_Array[count++] = lifeTime;

		// Ver6
		m_Array[count++] = centerX + offset;
		m_Array[count++] = centerY - offset;
		m_Array[count++] = 0;

		// Ver1의 방향벡터
		m_Array[count++] = ranVelx;	// x
		m_Array[count++] = ranVely;	// y
		m_Array[count++] = 0;	// z

		// 생성시간, 생존시간
		m_Array[count++] = startTime;
		m_Array[count++] = lifeTime;
	}
	

	glGenBuffers(1, &m_VBOLecture2);	// 개수 하나, 이름 지정
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(float), m_Array, GL_STATIC_DRAW);	
	delete(m_Array);
}

// 버텍스 생성2
void Renderer::GenQuads2(int n)
{
	float offset = 0.03f;				// 반지름
	float centerX, centerY, centerZ;	// 좌표
	int count = 0;						// 배열 인덱스
	float velX, velY, velZ;				// 가속도
	float startTime, lifeTime;			// 시작시간, 생명시간
	float ratio, amplitude;				// 주기 진폭
	float randValue;					// 랜덤 값
	float startTimeMax = 2.f;			// 최대 시작 시간
	float lifeTimeMax = 3.f;			// 최대 생명 시간
	float r, g, b, a;

	m_num = n * 6;						// 버텍스 개수
	m_size = n * 15 * 6;				// 원소 개수	15 == 버텍스의 원소 개수 6 == 쿼드당 버텍스 개수
	m_Array = new float[m_size];		// 배열 생성	

	srand(GetTickCount());

	for (int i = 0; i < n; i++)
	{							
		centerX = 0.f;
		centerY = 0.f;
		centerZ = 0.f;

		velX = (float)rand() / (float)RAND_MAX;
		velY = (float)rand() / (float)RAND_MAX;
		velZ = 0.f;

		startTime = ((float)rand() / (float)RAND_MAX) * startTimeMax;
		lifeTime = ((float)rand() / (float)RAND_MAX) * lifeTimeMax;
		ratio = (float)rand() / (float)RAND_MAX;
		amplitude = (float)rand() / (float)RAND_MAX * 2;

		randValue = (float)rand() / (float)RAND_MAX * 2;

		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
		a = 1.f;

		// Ver1
		m_Array[count++] = centerX + offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = centerZ;				// z

		m_Array[count++] = velX;				// velX
		m_Array[count++] = velY;				// velY
		m_Array[count++] = velZ;				// velZ
		
		m_Array[count++] = startTime;			// 생성시간
		m_Array[count++] = lifeTime;			// 생존시간
		m_Array[count++] = ratio;				// 주기
		m_Array[count++] = amplitude;			// 진폭

		m_Array[count++] = randValue;			// 값

		m_Array[count++] = r;			// r
		m_Array[count++] = g;			// g
		m_Array[count++] = b;			// b
		m_Array[count++] = a;			// a

		// Ver2
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = centerZ;				// z

		m_Array[count++] = velX;				// velX
		m_Array[count++] = velY;				// velY
		m_Array[count++] = velZ;				// velZ

		m_Array[count++] = startTime;			// 생성시간
		m_Array[count++] = lifeTime;			// 생존시간
		m_Array[count++] = ratio;				// 주기
		m_Array[count++] = amplitude;			// 진폭

		m_Array[count++] = randValue;			// 값

		m_Array[count++] = r;			// r
		m_Array[count++] = g;			// g
		m_Array[count++] = b;			// b
		m_Array[count++] = a;			// a

		// Ver3
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY - offset;	// y
		m_Array[count++] = centerZ;				// z

		m_Array[count++] = velX;				// velX
		m_Array[count++] = velY;				// velY
		m_Array[count++] = velZ;				// velZ

		m_Array[count++] = startTime;			// 생성시간
		m_Array[count++] = lifeTime;			// 생존시간
		m_Array[count++] = ratio;				// 주기
		m_Array[count++] = amplitude;			// 진폭

		m_Array[count++] = randValue;			// 값

		m_Array[count++] = r;			// r
		m_Array[count++] = g;			// g
		m_Array[count++] = b;			// b
		m_Array[count++] = a;			// a

		// Ver4
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY - offset;	// y
		m_Array[count++] = centerZ;				// z

		m_Array[count++] = velX;				// velX
		m_Array[count++] = velY;				// velY
		m_Array[count++] = velZ;				// velZ

		m_Array[count++] = startTime;			// 생성시간
		m_Array[count++] = lifeTime;			// 생존시간
		m_Array[count++] = ratio;				// 주기
		m_Array[count++] = amplitude;			// 진폭

		m_Array[count++] = randValue;			// 값

		m_Array[count++] = r;			// r
		m_Array[count++] = g;			// g
		m_Array[count++] = b;			// b
		m_Array[count++] = a;			// a

		// Ver5
		m_Array[count++] = centerX + offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = centerZ;				// z

		m_Array[count++] = velX;				// velX
		m_Array[count++] = velY;				// velY
		m_Array[count++] = velZ;				// velZ

		m_Array[count++] = startTime;			// 생성시간
		m_Array[count++] = lifeTime;			// 생존시간
		m_Array[count++] = ratio;				// 주기
		m_Array[count++] = amplitude;			// 진폭

		m_Array[count++] = randValue;			// 값

		m_Array[count++] = r;			// r
		m_Array[count++] = g;			// g
		m_Array[count++] = b;			// b
		m_Array[count++] = a;			// a

		// Ver6
		m_Array[count++] = centerX + offset;	// x
		m_Array[count++] = centerY - offset;	// y
		m_Array[count++] = centerZ;				// z

		m_Array[count++] = velX;				// velX
		m_Array[count++] = velY;				// velY
		m_Array[count++] = velZ;				// velZ

		m_Array[count++] = startTime;			// 생성시간
		m_Array[count++] = lifeTime;			// 생존시간
		m_Array[count++] = ratio;				// 주기
		m_Array[count++] = amplitude;			// 진폭

		m_Array[count++] = randValue;			// 값

		m_Array[count++] = r;			// r
		m_Array[count++] = g;			// g
		m_Array[count++] = b;			// b
		m_Array[count++] = a;			// a
	}

	glGenBuffers(1, &m_VBOLecture2);	// 개수 하나, 이름 지정
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(float), m_Array, GL_STATIC_DRAW);
	delete(m_Array);
}

// 버텍스 생성3
void Renderer::GenQuads3(int n)
{
	float offset = 0.5f;				// 반지름
	float centerX, centerY, centerZ;	// 좌표
	int count = 0;						// 배열 인덱스	

	m_num = n * 6;						// 버텍스 개수
	m_size = n * 5 * 6;				// 원소 개수	3 == 버텍스의 원소 개수 6 == 쿼드당 버텍스 개수
	m_Array = new float[m_size];		// 배열 생성	

	srand(GetTickCount());

	for (int i = 0; i < n; i++)
	{
		centerX = 0.f;
		centerY = 0.f;
		centerZ = 0.f;
	
		// Ver1
		m_Array[count++] = centerX + offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = centerZ;				// z
		m_Array[count++] = 1.f;					// u
		m_Array[count++] = 1.f;					// v
		
		// Ver2
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = centerZ;				// z
		m_Array[count++] = 0.f;					// u
		m_Array[count++] = 1.f;					// v

		// Ver3
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY - offset;	// y
		m_Array[count++] = centerZ;				// z
		m_Array[count++] = 0.f;					// u
		m_Array[count++] = 0.f;					// v

		// Ver4
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY - offset;	// y
		m_Array[count++] = centerZ;				// z
		m_Array[count++] = 0.f;					// u
		m_Array[count++] = 0.f;					// v

		// Ver5
		m_Array[count++] = centerX + offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = centerZ;				// z
		m_Array[count++] = 1.f;					// u
		m_Array[count++] = 1.f;					// v

		// Ver6
		m_Array[count++] = centerX + offset;	// x
		m_Array[count++] = centerY - offset;	// y
		m_Array[count++] = centerZ;				// z
		m_Array[count++] = 1.f;					// u
		m_Array[count++] = 0.f;					// v
	}

	glGenBuffers(1, &m_VBOLecture2);	// 개수 하나, 이름 지정
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);	// 작업할 대상 선택
	glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(float), m_Array, GL_STATIC_DRAW);	// VBOLEcture2에 데이터 할당
	delete(m_Array);
}
void Renderer::DrawQuads()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);		// 나중에 설명함  -> 0번지를 enable
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(0, 3 /*세개씩 끊어서 하나의 버텍스를 구성해라!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 3 /*다음 위치를 알려주는곳 (영어로는 stride)*/, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_num);	// 삼각형타입, ???, 그려야할 버텍스 개수	

	glDisableVertexAttribArray(0);	// 0번지 disable
}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_VBOGridMesh_Count = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBOGridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

void Renderer::Lecture3()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);		// 나중에 설명함  -> 0번지를 enable
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3 /*세개씩 끊어서 하나의 버텍스를 구성해라!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 3 /*다음 위치를 알려주는곳 (영어로는 stride)*/, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_VBOGridMesh_Count);	// 트라이앵글 형태, , 그려야할 버텍스 개수

	glDisableVertexAttribArray(0);	// 0번지 disable
}

void Renderer::Lecture4()
{
	glUseProgram(m_SimpleVelShader);

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	glUniform1f(uTime, offsetTime);
	offsetTime += 0.001f;

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6
		, (GLvoid*)(sizeof(float) * 3)/*float 크기만큼 3만큼 뒤로 밀어라*/);

	glDrawArrays(GL_TRIANGLES, 0, m_num);	// 삼각형타입, ???, 그려야할 버텍스 개수	

	glDisableVertexAttribArray(aPos);	// 0번지 disable
	glDisableVertexAttribArray(aVel);
}

void Renderer::Lecture5()
{
	glUseProgram(m_SimpleVelShader);

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	GLuint uRepeat = glGetUniformLocation(m_SimpleVelShader, "u_Repeat");

	glUniform1f(uTime, offsetTime);	
	offsetTime += 0.001f;

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_SimpleVelShader, "a_StartLife");	

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);	
	glEnableVertexAttribArray(aStartLife);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8
		, (GLvoid*)(sizeof(float) * 3)/*float 크기만큼 3만큼 뒤로 밀어라*/);
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_num);	// 삼각형타입, ???, 그려야할 버텍스 개수	

	glDisableVertexAttribArray(aPos);	// 0번지 disable
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
}

void Renderer::Lecture6()
{
	GLuint shader = m_Simple2Shader;	// 렌더링에 사용할 세이더를 지정  -> 여기서는 Simple2Shader를 쓴다

	glEnable(GL_BLEND);		// 블렌딩을 쓰겠다
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// 알파블렌딩을 쓰겠다

	glUseProgram(shader);	// 해당 쉐이더를 사용
	
	GLuint aPos = glGetAttribLocation(shader, "a_Position");	// Simple2Shader에 지정된 쉐이더 glGetAttribLocation : a_Position와 연동
	GLuint aVel = glGetAttribLocation(shader, "a_Vel");
	GLuint aStartLifeRatioAmp = glGetAttribLocation(shader, "a_StartLifeRatioAmp");
	GLuint aRandVal = glGetAttribLocation(shader, "a_RandVal");
	GLuint aColor = glGetAttribLocation(shader, "a_Color");

	GLuint uTime = glGetUniformLocation(shader, "u_Time");	// glGetUniformLocation : uniform 변수 u_Time 연결  uniform 변수는 외부 변화된 값을 받아온다

	glUniform1f(uTime, offsetTime);	// glUniform1f(받는 변수, 넘기는 변수)
	offsetTime += 0.0001f;
	
	glEnableVertexAttribArray(aPos);	// 배열 타입으로 어트리뷰트 값을 사용하게 한다
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLifeRatioAmp);
	glEnableVertexAttribArray(aRandVal);
	glEnableVertexAttribArray(aColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);	// aPos의 id값을 3개씩 읽고, 15칸씩 stride한다. 
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));	// aVel의 id값을 3개씩 읽고 15칸씩 stride하고 , 맨처음에 sizeof(float)*3 (세칸)만큼 건너 띄어서 읽는다
	glVertexAttribPointer(aStartLifeRatioAmp, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aRandVal, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_num);	// 삼각형타입, ???, 그려야할 버텍스 개수	

	glDisableVertexAttribArray(aPos);	
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLifeRatioAmp);
	glDisableVertexAttribArray(aRandVal);
	glDisableVertexAttribArray(aColor);
}

void Renderer::Lecture7()
{
	GLuint shader = m_Simple3Shader;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader);
	
	GLuint aPos = glGetAttribLocation(shader, "a_Position");	
	GLuint aUV = glGetAttribLocation(shader, "a_UV");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 2));

	glDrawArrays(GL_TRIANGLES, 0, m_num);	// 삼각형타입, ???, 그려야할 버텍스 개수

	glDisableVertexAttribArray(aPos);	
	glDisableVertexAttribArray(aUV);
}