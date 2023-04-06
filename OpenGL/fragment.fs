
#version 450 core
out vec4 FragColour;

in vec4 vertexColour;

void main()
{
    FragColour = vertexColour;
}