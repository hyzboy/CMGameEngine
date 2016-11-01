#version 150 core

uniform sampler2D Diffuse;
in vec2 FragmentTexCoord_Diffuse;

layout(location=0,index=0) out vec4 FragColor;

void main(void)
{
	vec4 DiffuseColor=texture(Diffuse,FragmentTexCoord_Diffuse);

	if(DiffuseColor.a<1)
        discard;

	FragColor=DiffuseColor;
}

