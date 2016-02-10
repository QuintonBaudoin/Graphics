#include "App.h"


App *App::instance = nullptr;
//App::App() {};
App *App::GetInstance()
{
	if(instance == nullptr)
	instance = new App();

	return instance;

}
int App::Init()
{
	m_view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_projection = glm::perspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_projectionViewMatrix = m_projection * m_view;

	if (glfwInit() == false)
		return -1;
	window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
	if (window == nullptr)
		glfwTerminate();

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf_s("GL: %i.%i\n", major, minor);

	return 1;
}


int App::LoadShaders(char vShaderPath[], char fShaderPath[])
{
	/*const char* vsSource = "#version 150\n \
					 in vec4 Position; \
					 in vec4 Colour; \
					 out vec4 vColour; \
					 uniform mat4 ProjectionView; \
					 void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

	const char* fsSource = "#version 150\n \
					 in vec4 vColour; \
					 out vec4 FragColor; \
					 void main()	{ FragColor = vColour; }";*/
#pragma region File In(Shader)
	std::string svsSource;
	std::string sfsSource;

	const char* vsSource;
	const char* fsSource;

	std::fstream file;
	file.open(vShaderPath);
	if (file.is_open())
	{

		while (file.good())
		{
			std::string tmp;
			std::getline(file, tmp);
			svsSource += tmp;
		}



	}
	else
		svsSource = "Failed to Open File\n";
	file.close();

	file.open(fShaderPath);
	if (file.is_open())
	{
		while (file.good())
		{
			std::string tmp;
			std::getline(file, tmp);
			sfsSource += tmp;
		}
	}
	else
		sfsSource = "Failed to Open File\n";

	vsSource = "#version 150\n \
		in vec4 Position; \
		in vec4 Colour; \
		out vec4 vColour; \
		uniform mat4 ProjectionView; \
		void main() { vColour = Colour; gl_Position = ProjectionView * Position; ";
	fsSource = "#version 150\n \
					 in vec4 vColour; \
					 out vec4 FragColour; \
					 void main()	{ FragColour = vColour; }";
	/*fsSource = sfsSource.c_str();
	vsSource = svsSource.c_str();*/
	file.close();
	printf("vsSource \n  #version 150\n \
		in vec4 Position; \
		in vec4 Colour; \
		out vec4 vColour; \
		uniform mat4 ProjectionView; \
		void main() { vColour = Colour; gl_Position = ProjectionView * Position; }	 \n");
	printf("\n");
	printf(vsSource);printf("\n");
	printf("fsSource \n #version 150\n \
		in vec4 vColour; \
		out vec4 FragColor; \
		void main() { FragColor = vColour; }	 \n");
	printf("\n");
	printf(fsSource);printf("\n");
	printf("-----------------------\n");


#pragma endregion

	
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, (const char**)&vsSource, 0);
	glCompileShader(vs);
	glShaderSource(fs, 1, (const char**)&fsSource, 0);
	glCompileShader(fs);

	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, vs);
	glAttachShader(m_ProgramID, fs);
	glBindAttribLocation(m_ProgramID, 0, "Position");
	glBindAttribLocation(m_ProgramID, 1, "Colour");
	glLinkProgram(m_ProgramID);
	
	int success = GL_FALSE;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(m_ProgramID, infoLogLength, 0, infoLog);
		printf("ERROR: Failed to link shader program!\n %s\n", infoLog);
		delete[] infoLog;
		return -1;
	}
	return 1;
}

void App::AddTriangle()
{
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

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * 3, &pvertex[0].x, GL_STATIC_DRAW);

	unsigned int pindices[3];
	pindices[0] = 0;
	pindices[1] = 1;
	pindices[2] = 2;

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, pindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(.25f, .25f, .25f, 1);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ProgramID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_ProgramID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);
}

void App::Update()
{
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
	
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//	return 1;
	}
}
void App::Term()
{


	glfwDestroyWindow(window);
	glfwTerminate();

	delete[] instance;
}