#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Shader.h>			//Shader
#include<hgl/object/FlowObject.h>		//FlowObject

using namespace hgl;
using namespace hgl::graph;

const char vertex_shader[]=		"#version 330\n"
								"\n"
								"in vec4 vertex;\n"
								"in vec4 color;\n"
								"\n"
								"out vec4 fragment_color;\n"
								"\n"
								"void main(void)\n"
								"{\n"
								"	gl_Position = vertex;\n"
								"	fragment_color=color;\n"
								"}";

const char fragment_shader[]=	"#version 330\n"
								"\n"
								"in vec4 fragment_color;\n"
								"\n"
								"layout(location = 0, index = 0) out vec4 fragColor;\n"
								"\n"
								"void main(void)\n"
								"{\n"
								"	fragColor = fragment_color;\n"
								"}";

// Points of a triangle.
float points[]={0.0f, 0.0f, 0.0f,1.0f,
				0.5f, 0.0f, 0.0f,1.0f,
				0.0f, 0.5f, 0.0f,1.0f};

float colors[]={1.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f};

class TestObject:public FlowObject
{
	Shader *shader;

	uint vao;

	uint vertex_vbo;
	uint color_vbo;

	int vertex_location;
	int color_location;

public:

	TestObject()
	{
		SetClearColor(0,0,0);

		shader=CreateShader(vertex_shader,fragment_shader);

        if(!shader)
        {
            fos=fosExitGame;
            return;
        }

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		vertex_location=shader->GetAttribLocation("vertex");
		color_location=shader->GetAttribLocation("color");

		shader->Use();

		glGenBuffers(1,&vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vertex_vbo);

		glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(float), (float *)points, GL_STATIC_DRAW);

		glVertexAttribPointer(vertex_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(vertex_location);

		glGenBuffers(1,&color_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,color_vbo);

		glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(float), (float *)colors, GL_STATIC_DRAW);

		glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(color_location);
	}

	~TestObject()
	{
		glDeleteBuffers(1,&color_vbo);
		glDeleteBuffers(1,&vertex_vbo);
		glDeleteVertexArrays(1,&vao);

		delete shader;
	}

	void Draw()
	{
		ClearScreen();

		glDrawArrays(GL_TRIANGLES,0,3);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("第一个三角形","FirstTriangle",new TestObject());
