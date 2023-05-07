
#include "Shader.h"

Shader::Shader(const char* vPath, const char* fPath)
{
    std::string vCode;
    std::string fCode;

    std::ifstream vShader;
    std::ifstream fShader;
    
    //allow throwing exceptions - do I want this - do I just allow a crash on dodgy shader?
    //want to handle exceptions in an editor setting but in a game setting what's best?
    vShader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShader.open(vPath);
        fShader.open(fPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShader.rdbuf();
        fShaderStream << fShader.rdbuf();

        vShader.close();
        fShader.close();

        vCode = vShaderStream.str();
        fCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "Error::Shader::FILE_COULD_NOT_BE_READ"<< std::endl;
    }

    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();

    unsigned int vertex = initialise(GL_VERTEX_SHADER, vShaderCode);
    unsigned int fragment = initialise(GL_FRAGMENT_SHADER, fShaderCode);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    //refactor out of here: can have an init shaders but even then this error checking is cumbersome
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Error: Shader: Program: Linking Failed:\n" << infoLog << std::endl;
	}

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::initialise(GLenum shaderType, const char* shaderSource)
{
    unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL); //shader object, number of strigns as source code, source code, ?
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error: Shader: Compilation Failed:\n" << infoLog << std::endl;
	}

	return shader;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::dispose()
{
    glDeleteProgram(ID);
    ID = 0;
}

void Shader::setBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name), (int) value);
}

void Shader::setInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setMatrix(const char* name, glm::f32* value) const
{
    //location, number of matrices, transpose?, matrix
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, value);
}