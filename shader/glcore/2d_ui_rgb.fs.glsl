#version 150 core

uniform sampler2D Diffuse;
in vec2 FragmentTexCoord_Diffuse;

layout(location=0,index=0) out vec4 FragColor;

void main(void)
{
	FragColor=texture(Diffuse,FragmentTexCoord_Diffuse);
}
