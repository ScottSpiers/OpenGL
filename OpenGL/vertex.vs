
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 tex;

out vec4 vertexColour;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    vertexColour = vec4(abs(gl_Position.xyz -0.5f), 1.0f);
    texCoord = tex;
}