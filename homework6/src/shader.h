#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


const char *basicLightingVertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
"    Normal = mat3(transpose(inverse(model))) * aNormal;  \n"
"    \n"
"    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";
const char *lampVertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\n";
const char *gouraudVertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"\n"
"out vec3 LightingColor; \n"
"\n"
"uniform vec3 viewPos;\n"
"uniform vec3 materialAmbient;\n"
"uniform vec3 materialDiffuse;\n"
"uniform vec3 materialSpecular;\n"
"uniform float materialShininess;\n"
"uniform vec3 lightPosition;\n"
"uniform vec3 lightAmbient;\n"
"uniform vec3 lightDiffuse;\n"
"uniform vec3 lightSpecular;\n"
"uniform vec3 lightColor;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"    \n"
"    vec3 Position = vec3(model * vec4(aPos, 1.0));\n"
"    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"    \n"
"    // 环境光 \n"
"    vec3 ambient = lightAmbient * materialAmbient;\n"
"  	\n"
"    // 漫反射 \n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPosition - Position);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = lightDiffuse * (diff * materialDiffuse);\n"
"    \n"
"    // 镜面反射 \n"
"    vec3 viewDir = normalize(viewPos - Position);\n"
"    vec3 reflectDir = reflect(-lightDir, norm);  \n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);\n"
"    vec3 specular = lightSpecular * (spec * materialSpecular);    \n"
"\n"
"    LightingColor = ambient + diffuse + specular;\n"
"}\n";


const char *basicLightingFragmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec3 FragPos;  \n"
"in vec3 Normal;  \n"
"  \n"
"uniform vec3 viewPos;\n"
"uniform vec3 materialAmbient;\n"
"uniform vec3 materialDiffuse;\n"
"uniform vec3 materialSpecular;\n"
"uniform float materialShininess;\n"
"uniform vec3 lightPosition;\n"
"uniform vec3 lightAmbient;\n"
"uniform vec3 lightDiffuse;\n"
"uniform vec3 lightSpecular;\n"
"uniform vec3 objectColor;\n"
"\n"
"void main()\n"
"{\n"
"    // 环境光 \n"
"    vec3 ambient = lightAmbient * materialAmbient;\n"
"  	\n"
"    // 漫反射 \n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPosition - FragPos);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = lightDiffuse * (diff * materialDiffuse);\n"
"    \n"
"    // 镜面反射 \n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    vec3 reflectDir = reflect(-lightDir, norm);  \n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);\n"
"    vec3 specular = lightSpecular * (spec * materialSpecular);  \n"
"        \n"
"    vec3 result = (ambient + diffuse + specular)*objectColor;\n"
"    FragColor = vec4(result, 1.0);\n"
"}\n";
const char *lampFramgmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0); // set alle 4 vector values to 1.0\n"
"}\n";
const char *gouraudFragmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec3 LightingColor; \n"
"\n"
"uniform vec3 objectColor;\n"
"\n"
"void main()\n"
"{\n"
"   FragColor = vec4(LightingColor * objectColor, 1.0);\n"
"}\n";


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
			glShaderSource(vertex, 1, &basicLightingVertexShader, NULL);
		}
		else if (a == 2) {
			glShaderSource(vertex, 1, &lampVertexShader, NULL);
		}
		else if (a == 3) {
			glShaderSource(vertex, 1, &gouraudVertexShader, NULL);
		}
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		if (a == 1) {
			glShaderSource(fragment, 1, &basicLightingFragmentShader, NULL);
		}
		else if (a == 2) {
			glShaderSource(fragment, 1, &lampFramgmentShader, NULL);
		}
		else if (a == 3) {
			glShaderSource(fragment, 1, &gouraudFragmentShader, NULL);
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

	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
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