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
	float x, y, z, w; ///Vector 4 position 
	float r, g, b, a; /// Vector 4 color

	MyVertex(float a_x, float a_y, float a_z,
	 float a_w, float a_r, float a_g, float a_b, float a_a)
	{
		x = a_x;
		y = a_y;
		z = a_z;
		w = a_w;

		r = a_r;
		g = a_g;
		b = a_b;
		a = a_a;


	};
	MyVertex(float a_x, float a_y, float a_z, float a_w)
	{
		x = a_x;
		y = a_y;
		z = a_z;
		w = a_w;

		r = 1;
		g = 1;
		b = 1;
		a = 1;


	};
	MyVertex()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;

		r = 1;
		g = 1;
		b = 1;
		a = 1;
	}
};



struct MyIndicies
{
	unsigned a, b, c; /// Index order
	MyIndicies(unsigned a_a, unsigned a_b, unsigned a_c) { a = a_a; b = a_b, c = a_c; }
	MyIndicies() { a = b = c = 0; }
};

struct OpenGLInfo
{
	unsigned int m_VAO; ///Vertex Array Object
	unsigned int m_VBO; /// Vertex Buffer Object
	unsigned int m_IBO; ///Index Buffer Object
	unsigned int m_index_count; /// Number of index'ss
};

///Singleton
class App
{
	static App* instance;

public:
	static App *GetInstance();
	
	///Initiates the program and Context Window
	int Init();
	///Loads shaders from file
	int LoadShaders(char vShaderPath[], char  fShaderPath[]);

	///Generates GlInfo based on verticies and indicies passed in
	OpenGLInfo LoadMesh(MyVertex* verts, unsigned nverts, MyIndicies* inds, unsigned ninds);
	void DrawTriangle(OpenGLInfo info);
	void DrawLines(OpenGLInfo info);

	///Loads OBJ and returns its shape_t
	std::vector<tinyobj::shape_t> OBJLoader();
	///draws Obj using Shape_t
	void App::DrawObj(std::vector<tinyobj::shape_t> &shapes);

	///Application update
	void Update();
	///Application term
	void Term();


	///Returns the shader
	unsigned int GetProgram()
	{
		return m_Shader;
	}
	///Returns the projection view matrix
	glm::mat4 GetProjectionViewMatrix()
	{
		return m_projectionViewMatrix;
	}





private:
	/// I did say it was a singleton so yeah. No constructor for you
	App() {};
	glm::mat4 m_view;// = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	glm::mat4 m_projection;// = glm::perspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 1000.f);
	glm::mat4 m_projectionViewMatrix;// = m_projection * m_view;
	GLFWwindow* window;


	

	unsigned int m_Shader;

	float m_deltaTime;

	//std::vector<OpenGLInfo> m_GLInfo;

	///Objectifies my verts,indies, and gl info
	class Shape
	{
	public:
		MyVertex* m_Verts;  //Verticies
		MyIndicies* m_Inds; //Indicies
		OpenGLInfo m_Mesh;  //OpenGLInfo

		Shape(MyVertex* verts, MyIndicies* inds);
	};

	class NoiseGrid
	{
	public:
		unsigned int m_Size;
		unsigned int m_vCount;
		unsigned int m_iCount;

		unsigned int m_PerlinTexture;

		MyVertex* m_Verts;  //Verticies
		MyIndicies* m_Inds; //Indicies
		

		NoiseGrid(unsigned int size);
		OpenGLInfo m_Mesh;  //OpenGLInfo
	};

	class Cube
	{
	public:
		unsigned int m_Size;
		unsigned int m_vCount;
		unsigned int m_iCount;

		MyVertex* m_Verts;  //Verticies
		MyIndicies* m_Inds; //Indicies


		Cube(unsigned int length, unsigned int width, unsigned height);
		OpenGLInfo m_Mesh;  //OpenGLInfo
	};

};