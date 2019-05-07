#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//顶点着色器
const char *vertexShaderSource3 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"   ourColor = aColor;\n"
"}\0";
const char *vertexShaderSource4 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aColor;\n"
"out vec2 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"   ourColor = aColor;\n"
"}\0";

//片段着色器
const char *fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);;\n"
"}\n\0";
const char *fragmentShaderSource4 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 ourColor;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, ourColor);\n"
"}\n\0";

//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

//片段着色器
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";
const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
class Shader
{
public:
	unsigned int ID;

	
	Shader() {
		ID = glCreateProgram();
	}
	void setShader(int a)
	{
		
		unsigned int vertex, fragment;
		
		vertex = glCreateShader(GL_VERTEX_SHADER);
		if (a == 1) {
			glShaderSource(vertex, 1, &vertexShaderSource3, NULL);
		}
		else if(a==2) {
			glShaderSource(vertex, 1, &vertexShaderSource4, NULL);
		}
		else {
			glShaderSource(vertex, 1, &vertexShaderSource, NULL);
		}
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		if (a == 1) {
			glShaderSource(fragment, 1, &fragmentShaderSource3, NULL);
		}
		else if(a==2) {
			glShaderSource(fragment, 1, &fragmentShaderSource4, NULL);
		}
		else {
			glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
		}
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		
		
		
		
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	
	void use()
	{
		glUseProgram(ID);
	}

	
	

private:

	
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif