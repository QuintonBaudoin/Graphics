#pragma once
#include "stdio.h"
#include <gl_core_4_4.h> 
#include <GLFW/glfw3.h> 
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>
#include<iostream>
#include <fstream>

#define BUFFER_OFFSET(i) ((void*)(i))

struct MyVertex
{
	float x, y, z;        //Vertex
	float nx, ny, nz;     //Normal
};
class App
{
	static App* instance;

public:
	static App *GetInstance();
	//enum ShaderType{Vertex, Fragment};

	int Init();

	int LoadShaders(char vShaderPath[], char  fShaderPath[]);
	void AddTriangle();
	void Update();
	void Term();
	//void CreateShader(ShaderType,char[],char[]);

private:
	App() {};
	glm::mat4 m_view;// = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	glm::mat4 m_projection;// = glm::perspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 1000.f);
	glm::mat4 m_projectionViewMatrix;// = m_projection * m_view;
	GLFWwindow* window;
	//int m_ProgramID;

	unsigned int m_VAO, m_VBO, m_IBO, m_ProgramID;

	


};