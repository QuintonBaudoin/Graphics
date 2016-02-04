#include "stdio.h"
#include <gl_core_4_4.h> 
#include <GLFW/glfw3.h> 
#include <Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>
#include "MyApplication.h"

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


int main()
{
	std::vector<tinyobj::shape_t> shapes;

		Application *theApp = new Application();
		
		if(theApp->startup() == true) 
		{
			while (theApp->update() == true)
			{
				theApp->draw();
			}
			theApp->shutdown();
		}

		delete theApp;
	return 0;
}