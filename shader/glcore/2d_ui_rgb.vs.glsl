#version 150 core

uniform mat4 ModelViewProjectionMatrix;

layout(location=0) in vec2 Vertex;

in vec2 TexCoord_Diffuse;
out vec2 FragmentTexCoord_Diffuse;

void main(void)
{
	FragmentTexCoord_Diffuse=TexCoord_Diffuse;

	gl_Position=vec4(Vertex,1.0,1.0)*ModelViewProjectionMatrix;
}

