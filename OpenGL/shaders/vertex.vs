
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out vec4 vertexColour;
out vec3 fragPos;
out vec3 Normal;
out vec2 texCoord;

uniform mat3 normalMat;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    fragPos = vec3(model * vec4(pos, 1.0));
    Normal = normalMat * normal;
    texCoord = tex;
}