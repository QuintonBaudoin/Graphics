#include "App.h"


App *App::instance = nullptr;
//App::App() {};
App *App::GetInstance()
{
	if (instance == nullptr)
		instance = new App();

	return instance;

}
int App::Init()
{

	m_view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0), glm::vec3(0, 1, 0));
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
int App::LoadShaders(char vShaderPath[], char fShaderPath[])
{
	const char* vsSource;
	const char* fsSource;

#pragma region File In(Shader)
	std::string svsSource;
	std::string sfsSource;


	std::fstream file;
	file.open(vShaderPath);
	if (file.is_open())
	{

		while (file.good())
		{
			std::string buff;
			while (std::getline(file, buff))
			{
				svsSource = svsSource + buff + "\n";
			}
		}



	}
	else
	{
		svsSource = "Failed to Open File\n";
		printf(svsSource.c_str());

		return -1;
	}
	file.close();
	file.open(fShaderPath);
	if (file.is_open())
	{
		while (file.good())
		{
			std::string buff;
			while (std::getline(file, buff))
			{
				sfsSource = sfsSource + buff + "\n";
			}
		}
	}
	else
	{
		sfsSource = "Failed to Open File\n";
		printf(sfsSource.c_str());
		return-1;
	}

	
	file.close();

	fsSource = sfsSource.c_str();
	vsSource = svsSource.c_str();


	//file.close();
	//printf("vsSource \n  #version 150\n \
	//	in vec4 Position; \
	//	in vec4 Colour; \
	//	out vec4 vColour; \
	//	uniform mat4 ProjectionView; \
	//	void main() { vColour = Colour; gl_Position = ProjectionView * Position; }	 \n");
	//printf("\n");
	//printf(vsSource);printf("\n");
	//printf("fsSource \n #version 150\n \
	//	in vec4 vColour; \
	//	out vec4 FragColor; \
	//	void main() { FragColor = vColour; }	 \n");
	//printf("\n");
	//printf(fsSource);printf("\n");
	//printf("-----------------------\n");


#pragma endregion

	//
	//unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	//unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

	//glShaderSource(vs, 1, (const char**)&vsSource, 0);
	//glCompileShader(vs);
	//glShaderSource(fs, 1, (const char**)&fsSource, 0);
	//glCompileShader(fs);

	//m_ProgramID = glCreateProgram();
	//glAttachShader(m_ProgramID, vs);
	//glAttachShader(m_ProgramID, fs);
	//glBindAttribLocation(m_ProgramID, 0, "Position");
	//glBindAttribLocation(m_ProgramID, 1, "Colour");
	//glLinkProgram(m_ProgramID);
	//
	//int success = GL_FALSE;
	//glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	//if (success == GL_FALSE)
	//{
	//	int infoLogLength = 0;
	//	glGetShaderiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	//	char* infoLog = new char[infoLogLength];

	//	glGetShaderInfoLog(m_ProgramID, infoLogLength, 0, infoLog);
	//	printf("ERROR: Failed to link shader program!\n %s\n", infoLog);
	//	delete[] infoLog;
	//	return -1;
	//}

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
		printf("Error: failed to link shader program! \n");
		printf("%s \n", infoLog);
		printf("\n");
		delete[] infoLog;
	}

	return 1;
}

void App::AddTriangle(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d)
{


#pragma region Triangle Vetex
	MyVertex pvertex[4];
	//VERTEX 1
	pvertex[0].x = a.x;
	pvertex[0].y = a.y;
	pvertex[0].z = a.z;
	pvertex[0].w = a.w;

	//VERTEX 2
	pvertex[1].x = b.x;
	pvertex[1].y = b.y;
	pvertex[1].z = b.z;
	pvertex[1].w = b.w;

	//VERTEX 3
	pvertex[2].x = c.x;
	pvertex[2].y = c.y;
	pvertex[2].z = c.z;
	pvertex[2].w = c.w;

	//VERTEX 4
	pvertex[3].x = d.x;
	pvertex[3].y = d.y;
	pvertex[3].z = d.z;
	pvertex[3].w = d.w;

#pragma endregion
	unsigned int pindices[6];
	pindices[0] = 0;
	pindices[1] = 1;
	pindices[2] = 2;
	pindices[3] = 3;
	pindices[4] = 2;
	pindices[5] = 1;

	glGenVertexArrays(1, &m_VAO);///Generates Vertex array as m_VAO
	glBindVertexArray(m_VAO);/// Binds m_VAO

	glGenBuffers(1, &m_VBO);///Generates Vertex Buffer as m_VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);///Binds m_VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * 4, &pvertex[0], GL_STATIC_DRAW);///buffers data from pvertrex starting at the first index



	glGenBuffers(1, &m_IBO);///Generates index buffer as m_IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);///Binds m_IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &pindices[0], GL_STATIC_DRAW);///Buffers data from pindices starting at first index

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(.0f, .0f, .0f, 0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ProgramID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_ProgramID, "ProjectionView");
	//unsigned int deltaTimeUniform = glGetUniformLocation(m_deltaTime, "DeltaTime");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));
	//glUniform2fv(deltaTimeUniform, 1, false);

	
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);

}


std::vector<tinyobj::shape_t> App::OBJLoader()
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	tinyobj::LoadObj(shapes, materials, err, "dragon.obj", "dragon.obj");
	printf(err.c_str());
	return shapes;
}

void App::DrawObj(std::vector<tinyobj::shape_t> &shapes)
{
	std::vector<OpenGLInfo> m_GLInfo;
	
	m_GLInfo.resize(shapes.size());
	/// where as i is mesh index
	for (unsigned int i = 0; i < shapes.size(); i++)
	{
		glGenVertexArrays(1, &m_GLInfo[i].m_VAO);
		glGenBuffers(1, &m_GLInfo[i].m_VBO);
		glGenBuffers(1, &m_GLInfo[i].m_IBO);
		glBindVertexArray(m_GLInfo[i].m_VAO);

		unsigned int floatCount = shapes[i].mesh.positions.size();
		floatCount += shapes[i].mesh.normals.size();
		floatCount += shapes[i].mesh.texcoords.size();

		std::vector<float> vextexData;
		vextexData.reserve(floatCount);

		vextexData.insert(vextexData.end(), shapes[i].mesh.positions.begin(), shapes[i].mesh.positions.end());
		vextexData.insert(vextexData.end(), shapes[i].mesh.normals.begin(), shapes[i].mesh.normals.end());

		m_GLInfo[i].m_index_count = shapes[i].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, m_GLInfo[i].m_VBO);
		glBufferData(GL_ARRAY_BUFFER,
			vextexData.size() * sizeof(float),
			vextexData.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLInfo[i].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			shapes[i].mesh.indices.size() * sizeof(unsigned int),
			shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
			(void*)(sizeof(float)*shapes[i].mesh.positions.size()));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(m_ProgramID);
		int view_proj_uniform = glGetUniformLocation(m_ProgramID, "projection_view");
		glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_projection);
		for (unsigned int i = 0; i < m_GLInfo.size(); ++i)
		{
			glBindVertexArray(m_GLInfo[i].m_VAO);
			glDrawElements(GL_TRIANGLES, m_GLInfo[i].m_index_count, GL_UNSIGNED_INT, 0);
		}

	}
}

void App::Update()
{
	float currentTime, Time = 0.0f;
	glm::vec4 a(0.f, 0.f, 0.f, 1.f);
	glm::vec4 b(2.f, 0.f, 0.f, 1.f);
	glm::vec4 c(0.f, 2.f, 0.f, 1.f);
	glm::vec4 d(2.f, 2.f, 0.f, 1.f);
	std::vector<tinyobj::shape_t> obj = OBJLoader();

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		currentTime = glfwGetTime();
		m_deltaTime = currentTime - Time;
		Time = currentTime;

		//glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::cout << m_deltaTime << std::endl;
		AddTriangle(a,b,c,d);
		//
	   DrawObj(obj);
		glfwSwapBuffers(window);
		glfwPollEvents();


	}
}
void App::Term()
{


	glfwDestroyWindow(window);
	glfwTerminate();

	delete[] instance;
}


void App::Triangle::AddTriangle(glm::vec4 a, glm::vec4 b, glm::vec4 c)
{

#pragma region Triangle Vetex
	MyVertex pvertex[3];
	//VERTEX 1
	pvertex[0].x = a.x;
	pvertex[0].y = a.y;
	pvertex[0].z = a.z;
	pvertex[0].w = a.w;

	//VERTEX 1
	pvertex[1].x = b.x;
	pvertex[1].y = b.y;
	pvertex[1].z = b.z;
	pvertex[1].w = b.w;

	//VERTEX 2
	pvertex[2].x = c.x;
	pvertex[2].y = c.y;
	pvertex[2].z = c.z;
	pvertex[2].w = c.w;

#pragma endregion
	unsigned int pindices[3];
	pindices[0] = 0;
	pindices[1] = 1;
	pindices[2] = 2;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * 3, &pvertex[0].x, GL_STATIC_DRAW);



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

	glUseProgram(App::GetInstance()->GetProgram());
	unsigned int projectionViewUniform = glGetUniformLocation(App::GetInstance()->GetProgram(), "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(App::GetInstance()->GetProjectionViewMatrix()));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);

}