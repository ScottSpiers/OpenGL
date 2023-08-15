
#version 450 core

out vec4 fragColour;
in vec4 vertexColour;
in vec2 texCoord;

uniform sampler2D tex;
uniform float time;

uniform vec3 objColour;
uniform vec3 lightColour;

void main()
{
   //FragColor = vertexColour * sin(time);
   vec3 colour = lightColour * objColour;
   fragColour = vec4(colour, 1.0);// * (vertexColour * sin(time));
}