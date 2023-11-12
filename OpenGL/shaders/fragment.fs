
#version 450 core

struct Material
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

struct Light
{
   vec3 position;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

out vec4 fragColour;
in vec4 vertexColour;
in vec3 Normal;
in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D tex;
uniform float time;

uniform vec3 objColour;
uniform vec3 viewPos;

uniform Material mat;
uniform Light light;

void main()
{
   //ambient
   vec3 ambient = light.ambient * mat.ambient;

   //diffuse
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - fragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * (diff * mat.diffuse);

   //specular
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
   vec3 specular = light.specular * (spec * mat.specular);
   
   vec3 result = ambient + diffuse + specular;
   fragColour = vec4(result, 1.0);
}