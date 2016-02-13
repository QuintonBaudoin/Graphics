#pragma once
#include "stdio.h"
#include <gl_core_4_4.h> 
#include <GLFW/glfw3.h> 
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>
#include<iostream>
#include <fstream>
#include "tiny_obj_loader.h"

#define BUFFER_OFFSET(i) ((void*)(i))



struct MyVertex
{
	float x, y, z, w;        //Vertex
	float nx, ny, nz;        //Normal
};

struct MyIndicies
{
	unsigned index[3];
	MyIndicies();
	MyIndicies(unsigned a, unsigned b, unsigned c) { index[0] = a; index[1] = b; index[2] = c; }
};

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};


class App
{
	static App* instance;

public:
	static App *GetInstance();
	//enum ShaderType{Vertex, Fragment};

	int Init();

	int LoadShaders(char vShaderPath[], char  fShaderPath[]);


	OpenGLInfo LoadMesh(MyVertex* verts, unsigned nverts, MyIndicies* inds, unsigned ninds);
	void Draw(OpenGLInfo info);


	std::vector<tinyobj::shape_t> OBJLoader();
	void Update();

	void Term();
	//void CreateShader(ShaderType,char[],char[]);

	unsigned int GetProgram()
	{
		return m_Shader;
	}
	glm::mat4 GetProjectionViewMatrix()
	{
		return m_projectionViewMatrix;
	}
private:
	App() {};
	glm::mat4 m_view;// = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	glm::mat4 m_projection;// = glm::perspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 1000.f);
	glm::mat4 m_projectionViewMatrix;// = m_projection * m_view;
	GLFWwindow* window;

	void App::DrawObj(std::vector<tinyobj::shape_t> &shapes);


	unsigned int m_Shader;

	float m_deltaTime;

	//std::vector<OpenGLInfo> m_GLInfo;

	///Gives object orientation to the triangle ( this will help me understand more... i hope)
	class Triangle
	{
	public:
		MyVertex m_Verts[3];
		MyIndicies* m_Inds;
		OpenGLInfo m_Mesh;

		Triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c, MyIndicies* inds);
	};

};