#include "App.h"
#include <iostream>

App *App::instance = nullptr;
//App::App() {};
App *App::GetInstance()
{
	if (instance == nullptr)
		instance = new App();

	return instance;

}




OpenGLInfo App::LoadMesh(MyVertex* verts, unsigned nverts, MyIndicies* inds, unsigned ninds)
{

	OpenGLInfo GLi; ///VBO,VAO,IBO,Index cout

	GLi.m_index_count = 3 * ninds;
	glGenVertexArrays(1, &GLi.m_VAO);///Generates Vertex array as m_VAO
	glBindVertexArray(GLi.m_VAO);/// Binds m_VAO

	glGenBuffers(1, &GLi.m_VBO);///Generates Vertex Buffer as m_VBO
	glBindBuffer(GL_ARRAY_BUFFER, GLi.m_VBO);///Binds m_VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * nverts, &verts[0].x, GL_STATIC_DRAW);///buffers data from pvertrex starting at the first index



	glGenBuffers(1, &GLi.m_IBO);///Generates index buffer as m_IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLi.m_IBO);///Binds m_IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MyIndicies) * ninds, &inds[0].a, GL_STATIC_DRAW);///Buffers data from pindices starting at first index

	///The fallowing code gives data from the verts(MyVertex) and puts feeds it to the shader threw Layout
	///0 = position
	///1 = color;

	///Gives the "Position" variable the values x,y,z,w by giving offset of 0 and variables of 4 ([0]->[3]
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	///Gives the "Color" variable the values r,g,b,a by giving offset of sizeof(float)*4 and variables of 4 ([4]->[7]
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (GLvoid*)((sizeof(float) * 4)));
	glEnableVertexAttribArray(1);

	///Clears buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	///



	return GLi;
}
void App::Draw(OpenGLInfo info)
{
	glUseProgram(m_Shader); /// Binds Shader
	/// Sets Uniform Values
	unsigned int projectionViewUniform = glGetUniformLocation(m_Shader, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));
	///Binds Vertex Array
	glBindVertexArray(info.m_VAO);
	///Draws Triangle from the OpenGLInfo passed in
	glDrawElements(GL_TRIANGLES, info.m_index_count, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
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
//I couldnt say exactly what is going on here.
//But i can paraphrase.
//
//It takes shapes variable and shufles threw it.
//Creating GlInfo for each triangle.
//
//then draws based on each GLInfo.
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

		glUseProgram(m_Shader);
		int view_proj_uniform = glGetUniformLocation(m_Shader, "projection_view");
		glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_projection);
		for (unsigned int i = 0; i < m_GLInfo.size(); ++i)
		{
			glBindVertexArray(m_GLInfo[i].m_VAO);
			glDrawElements(GL_TRIANGLES, m_GLInfo[i].m_index_count, GL_UNSIGNED_INT, 0);
		}

	}
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
int App::LoadShaders(char vShaderPath[], char fShaderPath[])
{

	const char* vsSource; ///Char pointer for the Vertex Shader
	const char* fsSource; /// Char pointer for the Fragment Shader



#pragma region File In(Shader)
	///Variables used to store the shader during file read
	std::string svsSource;
	std::string sfsSource;

	///File to be read from
	std::fstream file;
	///Opens file as path givin by vShaderPath
	file.open(vShaderPath);
	if (file.is_open()) ///Checks to see if file is open
	{

		while (file.good())/// While file is good
		{
			std::string buff; ///Buffer for pulling each line
			while (std::getline(file, buff))/// WHile there is a line to add to buff
			{
				svsSource = svsSource + buff + "\n"; ///transfers from buffer to string adding endline character
			}
		}



	}
	else ///Ife not oppend
	{
		svsSource = "Failed to Open File\n"; ///Sets Shader to error message
		printf(svsSource.c_str());///prints message

		return -1;
	}
	file.close(); ///closes file
	////The below code is the same as upper code. just variable differences


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

	//////
	file.close();
	///Sets the char pointers to shader files
	fsSource = sfsSource.c_str();
	vsSource = svsSource.c_str();

#pragma endregion

	unsigned int vs = glCreateShader(GL_VERTEX_SHADER); ///Creates vertex shader
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER); ///creates fragment shader

	glShaderSource(vs, 1, (const char**)&vsSource, 0); ///sets vertex shader as the file given above
	glCompileShader(vs);
	glShaderSource(fs, 1, (const char**)&fsSource, 0); ///sets fragment shader as the file given above
	glCompileShader(fs);

	m_Shader = glCreateProgram(); ///Creates shader program
	glAttachShader(m_Shader, vs);///Attaches shader to program
	glAttachShader(m_Shader, fs);///attaches shader to program
	glBindAttribLocation(m_Shader, 0, "Position");///Locates layout variable
	glBindAttribLocation(m_Shader, 1, "Color");///locates layout variable
	glLinkProgram(m_Shader); ///Links program with shader


	int success = GL_FALSE;
	glGetProgramiv(m_Shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(m_Shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(m_Shader, infoLogLength, 0, infoLog);
		printf("Error: failed to link shader program! \n");
		printf("%s \n", infoLog);
		printf("\n");
		delete[] infoLog;
	}

	return 1;
}
void App::Update()
{
	glClearColor(.0f, .0f, .0f, 0);
	glEnable(GL_DEPTH_TEST);
	float currentTime, Time = 0.0f;

	/// Establishing 4 Vec 4 Positions
	//glm::vec4 a(0.f, 0.f, 0.f, 1.f);
	//glm::vec4 b(2.f, 0.f, 0.f, 1.f);
	//glm::vec4 c(0.f, 2.f, 0.f, 1.f);
	//glm::vec4 d(2.f, 2.f, 0.f, 1.f);

	///Establishes my 4 verticies for my square
	MyVertex verta[4] =
	{
		{ 0.f, 0.f, 0.f, 1.f , 1.f, 0.f, 0.f, 1.f},  ///Bot left 0
		{ 2.f, 0.f, 0.f, 1.f , 0.f, 1.f, 0.f, 1.f},  ///Bot right 1
		{ 2.f, 2.f, 0.f, 1.f , 1.f, 0.f, 0.f, 1.f }, ///Top right 2		
		{ 0.f, 2.f, 0.f, 1.f , 0.f, 0.f, 1.f, 1.f}  ///Top left 3
	};

	///setablishes my 3 verticies for my triangle
	MyVertex vertb[3] =
	{
		{ 4.f, 2.f, 2.f, 1.f , 1.f, 0.f, 0.f, 1.f },
		{ 2.f, 4.f, 2.f, 1.f , 0.f, 1.f, 0.f, 1.f },
		{ 4.f, 4.f, 2.f, 1.f , 0.f, 0.f, 1.f, 1.f }
	};



	///Creating the Indices for my triangle
	MyIndicies* Tri = new MyIndicies(0, 1, 2);



	MyIndicies Squ[2] = { {0,1,2},{0,2,3} };


	///Creates 
	Shape* Square = new Shape(verta, Squ);
	Shape* Triangle = new Shape(vertb, Tri);


	Square->m_Mesh = LoadMesh(Square->m_Verts, 4, Square->m_Inds, 2);
	Triangle->m_Mesh = LoadMesh(Triangle->m_Verts, 3, Triangle->m_Inds, 1);


	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentTime = (float)glfwGetTime();
		m_deltaTime = currentTime - Time;
		Time = currentTime;

		//	std::cout << m_deltaTime * 100 << std::endl;

		Draw(Square->m_Mesh);
		Draw(Triangle->m_Mesh);

		//glEnable(GL_DEPTH_TEST);



		//std::cout << m_deltaTime << std::endl;

		//TriangleOne->AddTriangle(Model);

	   //DrawObj(obj);
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




App::Shape::Shape(MyVertex* verts, MyIndicies* inds)
{
	m_Verts = verts;
	m_Inds = inds;

}