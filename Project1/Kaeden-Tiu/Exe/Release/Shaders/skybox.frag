#version 330 core

out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube skybox;
uniform bool nightVision;

void main()
{
   FragColor = texture(skybox, TexCoord);
}