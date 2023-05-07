
#version 450 core

out vec4 fragColour;
in vec4 vertexColour;
in vec2 texCoord;

uniform sampler2D tex;
uniform float time;

void main()
{
   //FragColor = vertexColour * sin(time);
   fragColour = texture(tex, texCoord);// * (vertexColour * sin(time));
}