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
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	
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
		size, size, 0.f, 0.5, //Triangle1 ���ؽ� 3��
		-size, -size, 0.f, 0.5,
		size, size, 0.f, 0.5,
		size, -size, 0.f, 0.5 //Triangle2 ���ؽ� 3��
	};

	glGenBuffers(1, &m_VBORect);			// id�� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);	// GL_ARRAY_BUFFER ��� �۾���� �̵�
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);		// �۾��뿡�� ���� ���ϴ� �����͸� �Ѱ��ش� -> VBORect�� ���� ���ϴ� ������ �ְԵȴ�

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

	glGenBuffers(1, &m_VBORectColor);			// id�� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);	// GL_ARRAY_BUFFER ��� �۾���� �̵�
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);		// �۾��뿡�� ���� ���ϴ� �����͸� �Ѱ��ش� -> VBORect�� ���� ���ϴ� ������ �ְԵȴ�

	//lecture2
	
	float triangleVertex[]
		=
	{
		-1.0, 0.f, 0.f, 0.f, 1.0, 0.f, 1.0, 0.f, 0.f, // 9 floats		
	};

	glGenBuffers(1, &m_VBOLecture2);	// ���� �ϳ�, �̸� ���� 
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertex), triangleVertex, GL_STATIC_DRAW);
	*/

	GenQuads(1000);
	//CreateGridMesh();
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
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
	glUseProgram(m_SolidRectShader);	// m_SolidRectShader ���̴��� ���ڴ�.

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	GLuint uTheta = glGetUniformLocation(m_SolidRectShader, "u_Theta");
	
	offsetTheta += 0.001f;
	offsetTime += 0.0001f;
	
	glUniform1f(uTheta, offsetTheta);
	//glUniform1f(uTime, offsetTime);			//glUniform1f float�� �ϳ� �ְڴ� uTime�� ���� u_Time�� �������� ���� offset���� ��.

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");	
	int attribColor = glGetAttribLocation(m_SolidRectShader, "a_Color");

	glEnableVertexAttribArray(attribPosition);	// 0 ������ �����̼��� �ִ� ��Ʈ����Ʈ�� enable �ϰڴ�. SolidRect.vs�� a_Position�� ��밡���ϰ� ��.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 4 /*������ ��� �ϳ��� ���ؽ��� �����ض�!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 4 /*���� ��ġ�� �˷��ִ°� (����δ� stride)*/, 0);
	// 3���� ���� float ���� SolidRect.vs�� a_Position�� ���� ���� �ȴ�.

	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(attribColor, 4 /*������ ��� �ϳ��� ���ؽ��� �����ض�!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 4 /*���� ��ġ�� �˷��ִ°� (����δ� stride)*/, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribColor);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);		// ���߿� ������  -> 0������ enable
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(0, 3 /*������ ��� �ϳ��� ���ؽ��� �����ض�!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 3 /*���� ��ġ�� �˷��ִ°� (����δ� stride)*/, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);	// Ʈ���̾ޱ� ����, , ���ؽ� 3��

	glDisableVertexAttribArray(0);	// 0���� disable
}

// ���ؽ� ����
void Renderer::GenQuads(int n)
{
	float offset = 0.01f;	// ������
	float centerX, centerY;
	int count = 0;

	m_num = n * 6;		// ���ؽ� ����
	m_size = n * 6 * 3;	// ���� ����	
	m_Array = new float[m_size];	// �迭 ����	

	srand(GetTickCount());

	for (int i = 0; i < n; i++)
	{
		centerX = ((float)rand() / RAND_MAX ) * 2 - 1;	// �߽��� ���� -1 ~ 1
		centerY = ((float)rand() / RAND_MAX) * 2 - 1;	// �߽��� ���� -1 ~ 1

		// Ver1
		m_Array[count++] = centerX - offset;	// x
		m_Array[count++] = centerY + offset;	// y
		m_Array[count++] = 0;					// z
		// Ver2
		m_Array[count++] = centerX + offset;
		m_Array[count++] = centerY + offset;
		m_Array[count++] = 0;
		// Ver3
		m_Array[count++] = centerX - offset;
		m_Array[count++] = centerY - offset;
		m_Array[count++] = 0;
		// Ver4
		m_Array[count++] = centerX + offset;
		m_Array[count++] = centerY + offset;
		m_Array[count++] = 0;
		// Ver5
		m_Array[count++] = centerX - offset;
		m_Array[count++] = centerY - offset;
		m_Array[count++] = 0;
		// Ver6
		m_Array[count++] = centerX + offset;
		m_Array[count++] = centerY - offset;
		m_Array[count++] = 0;
	}
	

	glGenBuffers(1, &m_VBOLecture2);	// ���� �ϳ�, �̸� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(float), m_Array, GL_STATIC_DRAW);	
	delete(m_Array);
}

void Renderer::DrawQuads()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);		// ���߿� ������  -> 0������ enable
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(0, 3 /*������ ��� �ϳ��� ���ؽ��� �����ض�!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 3 /*���� ��ġ�� �˷��ִ°� (����δ� stride)*/, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_num);	// �ﰢ��Ÿ��, ???, �׷����� ���ؽ� ����	

	glDisableVertexAttribArray(0);	// 0���� disable
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
	glEnableVertexAttribArray(0);		// ���߿� ������  -> 0������ enable
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3 /*������ ��� �ϳ��� ���ؽ��� �����ض�!*/, GL_FLOAT, GL_FALSE, sizeof(float) * 3 /*���� ��ġ�� �˷��ִ°� (����δ� stride)*/, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_VBOGridMesh_Count);	// Ʈ���̾ޱ� ����, , �׷����� ���ؽ� ����

	glDisableVertexAttribArray(0);	// 0���� disable
}