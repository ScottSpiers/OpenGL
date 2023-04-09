
#version 450 core

out vec4 FragColor;
in vec4 vertexColour;

uniform float time;
void main()
{
   FragColor = vertexColour * sin(time);
}