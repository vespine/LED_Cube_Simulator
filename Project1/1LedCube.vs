#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 offsets[512];


void main()
{
	//vec3 offset = offsets[gl_InstanceID];
	//gl_Position = vec4(aPos + offset, 1.0);
	
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}