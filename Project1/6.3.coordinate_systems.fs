#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 ourColor;


uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = ourColor;
}