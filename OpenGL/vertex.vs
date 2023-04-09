
#version 450 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColour;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColour = vec4(abs(gl_Position.xyz - 0.5f), 1.0);
}