#include "stdio.h"
#include <gl_core_4_4.h> 
#include <GLFW/glfw3.h> 
#include <Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>
#include "MyApplication.h"
#include<iostream>
#define BUFFER_OFFSET(i) ((void*)(i))

namespace tinyobj
{

	typedef struct mesh_t
	{
		std::vector<float>			positions;
		std::vector<float>			normals;
		std::vector<float>			texcoords;
		std::vector<unsigned int>	indices;
		std::vector<int>			material_ids;
	};

	typedef struct shape_t
	{
		std::string			name;
		mesh_t				mesh;
	};


}

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

std::vector<OpenGLInfo> m_gl_info;
void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{
	m_gl_info.resize(shapes.size());
	for (unsigned int mesh_index = 0; mesh_index < shapes.size(); ++mesh_index) 
	{
		glGenVertexArrays(1, &m_gl_info[mesh_index].m_VAO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_VBO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_IBO);  
		glBindVertexArray(m_gl_info[mesh_index].m_VAO);

		unsigned int float_count = shapes[mesh_index].mesh.positions.size();  
		float_count += shapes[mesh_index].mesh.normals.size(); 
		float_count += shapes[mesh_index].mesh.texcoords.size();

		std::vector<float> vertex_data;		
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(),
			shapes[mesh_index].mesh.positions.begin(), 
			shapes[mesh_index].mesh.positions.end()); 
		
		vertex_data.insert(vertex_data.end(), 
			shapes[mesh_index].mesh.normals.begin(),
			shapes[mesh_index].mesh.normals.end());
		
		m_gl_info[mesh_index].m_index_count = 
			shapes[mesh_index].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_index].m_VBO);  
		glBufferData(GL_ARRAY_BUFFER, 
					vertex_data.size() * sizeof(float),
					vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int), 
					shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal data  
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, 
		(void*)(sizeof(float)*shapes[mesh_index].mesh.positions.size()));
		
		glBindVertexArray(0);  
		glBindBuffer(GL_ARRAY_BUFFER, 0);  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
}

struct MyVertex
{
	float x, y, z;        //Vertex
	float nx, ny, nz;     //Normal
};

struct Vertex
{
	glm::vec3 position;
	glm::vec4 colour;
};

////mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
////mat4 projection = glm::perspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 1000.f);
////mat4 projectionViewMatrix = projection * view;
////unsigned int m_programID;
////
////GLFWwindow *window;
//
//
//unsigned int m_VAO;
//unsigned int m_VBO;
//unsigned int m_IBO;
////void MakeTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
////{
////	Vertex* aoVertices = new Vertex[3];
////
////	aoVertices[0].position = a;
////	aoVertices[1].position = b;
////	aoVertices[2].position = c;
////	
////
////
////	unsigned int pindices[3];
////	pindices[0] = 0;
////	pindices[1] = 1;
////	pindices[2] = 2;
////
////	//begin: bind and fill vbo + ibo	
////	glGenBuffers(1, &m_VBO);
////	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
////	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, &aoVertices[0].position.x, GL_STATIC_DRAW);
////
////	glGenBuffers(1, &m_IBO);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
////	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, pindices, GL_STATIC_DRAW);
////
////	glGenVertexArrays(1, &m_VAO);
////	glBindVertexArray(m_VAO);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
////	glEnableVertexAttribArray(0);
////	glEnableVertexAttribArray(1);
////	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
////	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(12));
////
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
////
////	glBindVertexArray(0);
////	glBindBuffer(GL_ARRAY_BUFFER, 0);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
////
////
////	glClearColor(0.25f, 0.25f, 0.25f, 1);
////	glEnable(GL_DEPTH_TEST);
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////
////	glUseProgram(m_programID);
////	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "ProjectionView");
////	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projectionViewMatrix));
////
////	glBindVertexArray(m_VAO);
////	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
////
////
////	delete[] aoVertices;
////}
//
////function to create a grid
////void GenerateGrid(unsigned int rows, unsigned int cols)
////{	
////
////	Vertex* aoVertices = new Vertex[rows * cols];
////
////
////
////	for (unsigned int r = 0; r < rows; ++r)
////		for (unsigned int c = 0; c < cols; ++c)
////		{
////			aoVertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);
////			vec3 colour = vec3(sinf((c / (float)(rows - 1)) * (r / (float)rows - 1)));
////			aoVertices[r*cols + c].colour = vec4(colour, 1);
////	}
////
////	unsigned int* auiIndices = new unsigned int[(rows - 1)*(cols - 1) * 6];
////	//unsigned int index = 0;
////	//for (unsigned int r = 0; r < (rows - 1); r++)
////	//	for (unsigned int c = 0; r < (cols - 1); c++)
////	//{
////	//	//triangle 1
////	//	auiIndices[index++] = r*cols + c;
////	//	auiIndices[index++] = (r + 1) * cols + c;
////	//	auiIndices[index++] = (r + 1)*cols + (c + 1);
////
////	//	//// triangle 2
////	//	auiIndices[index++] = r * cols + c;
////	//	auiIndices[index++] = (r + 1) * cols + (c + 1);
////	//	auiIndices[index++] = r * cols + (c + 1);
////	//	
////	//}
////
////
////	
////
////	glGenBuffers(1, &m_VBO);
////	glGenBuffers(1, &m_IBO);
////
////	glGenVertexArrays(1, &m_VAO);
////	glBindVertexArray(m_VAO);
////
////	glBindVertexArray(0);
////	glBindBuffer(GL_ARRAY_BUFFER, 0);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
////
////	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
////	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
////
////
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
////	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 *	sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
////
////	glEnableVertexAttribArray(0);
////	glEnableVertexAttribArray(1);
////	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
////
////	glBindBuffer(GL_ARRAY_BUFFER, 0);
////	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
////	
//// 	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(sizeof(vec4)));
////	unsigned int indexCount = (rows - 1) * (cols - 1) * 6;
////	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
////
////	
////	delete[] aoVertices;
////}
////void CreateShader()
////{
////	const char* vsSource = "#version 410\n \
////							layout(location = 0) in vec4 Position; \
////							layout(location=1) in vec4 Colour;	\
////							out vec4 vColour;	\
////							uniform mat4 ProjectionView;	\
////							void main() { vColour = Colour; gl_Position = ProjectionView * Position;}";
////
////	const char* fsSource = "#version 410\n	\
////							in vec4 vColour;	\
////							out vec4 FragColour;\
////							void main() {FragColour = vColour;}";
////
////	int success = GL_FALSE;
////	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
////	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
////
////	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
////	glCompileShader(vertexShader);
////	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
////	glCompileShader(fragmentShader);
////
////	m_programID = glCreateProgram();
////	glAttachShader(m_programID, vertexShader);
////	glAttachShader(m_programID, fragmentShader);
////	glLinkProgram(m_programID);
////	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
////	if (success == GL_FALSE)
////	{
////		int infoLogLength = 0;
////		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
////		char* infoLog = new char[infoLogLength];
////
////		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
////		printf("error: failed to link shader program!\n");
////		printf("%s\n",infoLog);
////		delete[]infoLog;
////	}
////
////	glUseProgram(m_programID);
////	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "ProjectionView");
////
////	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projectionViewMatrix));
////
////
////	glDeleteShader(fragmentShader);
////	glDeleteShader(vertexShader);
////
////}
////int Startup()
////{
////	if (!glfwInit())
////		return -1;
////	
////	window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
////
////	if (window == nullptr)
////		glfwTerminate();
////
////	glfwMakeContextCurrent(window);
////
////	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
////	{
////		glfwDestroyWindow(window);
////		glfwTerminate();
////		return -3;
////	}
////	auto major = ogl_GetMajorVersion();
////	auto minor = ogl_GetMinorVersion();
////	printf_s("GL: %i.%i\n",major,minor);
////	
////	return 1;
////}
////
////int Update()
////{
////	
////	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
////	{
////		glClearColor(.25f, .25f, .25f, 1);
////		glEnable(GL_DEPTH_TEST);
////		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////
////		return 1;
////	}
////	return 0;
////}
////void Shutdown()
////{
////	std::printf("shutting down...");
////	Gizmos::destroy();
////	glfwDestroyWindow(window);
////	glfwTerminate();
////}

unsigned int g_VAO, g_VBO, g_IBO, g_shader;
int main()
{
	mat4 m_view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	mat4 m_projection = glm::perspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 1000.f);
	mat4 m_projectionViewMatrix = m_projection * m_view;

	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	//the rest of our code goes here!
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}


	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	printf_s("GL: %i.%i\n", major, minor);

	const char* vsSource = "#version 150\n \
					 in vec4 Position; \
					 in vec4 Colour; \
					 out vec4 vColour; \
					 uniform mat4 ProjectionView; \
					 void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

	const char* fsSource = "#version 150\n \
					 in vec4 vColour; \
                     out vec4 FragColor; \
					 void main()	{ FragColor = vColour; }";

	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, (const char**)&vsSource, 0);
	glCompileShader(vs);
	glShaderSource(fs, 1, (const char**)&fsSource, 0);
	glCompileShader(fs);

	g_shader = glCreateProgram();
	glAttachShader(g_shader, vs);
	glAttachShader(g_shader, fs);
	glBindAttribLocation(g_shader, 0, "Position");
	glBindAttribLocation(g_shader, 1, "Colour");
	glLinkProgram(g_shader);

	int success = GL_FALSE;
	glGetProgramiv(g_shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(g_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(g_shader, infoLogLength, 0, infoLog);
		printf("ERROR: Failed to link shader program!\n%s\n", infoLog);
		delete[] infoLog;
	}

	MyVertex pvertex[3];

	pvertex[0].x = 0.0;
	pvertex[0].y = 0.0;
	pvertex[0].z = 0.0;


	//VERTEX 1
	pvertex[1].x = 1.0;
	pvertex[1].y = 0.0;
	pvertex[1].z = 0.0;

	//VERTEX 2
	pvertex[2].x = 0.0;
	pvertex[2].y = 1.0;
	pvertex[2].z = 0.0;

	glGenVertexArrays(1, &g_VAO);
	glBindVertexArray(g_VAO);

	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * 3, &pvertex[0].x, GL_STATIC_DRAW);

	unsigned int pindices[3];
	pindices[0] = 0;
	pindices[1] = 1;
	pindices[2] = 2;
	
	glGenBuffers(1, &g_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, pindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glClearColor(.25f, .25f, .25f, 1);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(g_shader);
	unsigned int projectionViewUniform = glGetUniformLocation(g_shader, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	glBindVertexArray(g_VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);

	while()
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	//Startup();
	//CreateShader();
	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//MakeTriangle(glm::vec3(0,0,0), glm::vec3(.5,0,0), glm::vec3(0,.5,0));
	//while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	//{
	//	// our game logic and update code goes here!
	//	// so does our render code!
	//	glClearColor(0.25f, 0.25f, 0.25f, 1);
	//	glEnable(GL_DEPTH_TEST);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	//GL_COLOR_BUFFER_BIT tells OpenGL to wipe the back-buffer colors
	//	//GL_DEPTH_BUFFER_BIT clear the distance to the closest pixel. We do this that openGL loads are new images from frame to fram
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//	
	//}
	//glDeleteBuffers(1, &m_VBO);
	//glDeleteBuffers(1, &m_IBO);
	//glfwDestroyWindow(window);
	//glfwTerminate();
	////std::vector<tinyobj::shape_t> shapes;

	////	Application *theApp = new Application();
	////	
	////	if(theApp->startup() == true) 
	////	{
	////		while (theApp->update() == true)
	////		{
	////			theApp->draw();
	////		}
	////		theApp->shutdown();
	////	}

	////	delete theApp;
	return 0;
}