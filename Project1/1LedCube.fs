#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float red;
uniform float green;
uniform float blue;
uniform float alphaf;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
   FragColor = vec4(blue, green, red, alphaf);
}