#version 330 core


layout(location = 0) in vec3 aPos;

layout(location = 2) in vec2 aTex;

out vec2 TexCoord;

/*
uniform float x_mod;

void main()
{

	vec3 newPos = vec3(aPos.x + x_mod, aPos.y, aPos.z);

	gl_Position = vec4(newPos, 1.0);
}
*/

uniform mat4 projection;

uniform mat4 transform;

uniform mat4 view;

void main()
{

	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	TexCoord = aTex;

}

