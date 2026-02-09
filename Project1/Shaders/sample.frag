#version 330 core

uniform sampler2D tex0;
in vec2 TexCoord;
out vec4 FragColor;

void main()
{
	//FragColor =  vec4(0.7, 0.0, 0.0, 1.0); 

	FragColor = texture(tex0, TexCoord);
}