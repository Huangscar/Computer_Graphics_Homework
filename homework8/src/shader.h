#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//hw6
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

//hw7
const char *simpleDepthFragmentShader = "#version 330 core\n"
"\n"
"void main()\n"
"{             \n"
"}\n";
const char *simpleDepthVertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"uniform mat4 lightSpaceMatrix;\n"
"uniform mat4 model;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);\n"
"}\n";

const char *showDepthFragmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec2 TexCoords;\n"
"\n"
"uniform sampler2D fboAttachment;\n"
"uniform float nearPlane;\n"
"uniform float farPlane;\n"
"uniform int type;\n"
"\n"
"\n"
"float LinearizeDepth(float depth)\n"
"{\n"
"    float z = depth * 2.0 - 1.0; \n"
"    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	\n"
"}\n"
"\n"
"void main()\n"
"{             \n"
"    float depthValue = texture(fboAttachment, TexCoords).r;\n"
"	 if(type == 0) {\n"
"		FragColor = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0);\n"
"	 }\n"
"	 else {\n"
"		FragColor = vec4(vec3(depthValue), 1.0); \n"
"	 }\n"
"}\n";
const char *showDepthVertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoords;\n"
"\n"
"out vec2 TexCoords;\n"
"\n"
"void main()\n"
"{\n"
"    TexCoords = aTexCoords;\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\n";
const char *shadowMappingFragmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec3 color;\n"
"in vec4 FragPosLightSpace;\n"
"\n"
"\n"
"uniform sampler2D diffuseTexture;\n"
"uniform sampler2D shadowMap;\n"
"\n"
"uniform vec3 lightPosition;\n"
"uniform vec3 viewPos;\n"
"\n"
"float ShadowCalculation(vec4 fragPosLightSpace)\n"
"{\n"
"    //透视除法\n"
"    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
"    //0-1范围\n"
"    projCoords = projCoords * 0.5 + 0.5;\n"
"    //最近点的深度\n"
"    float closestDepth = texture(shadowMap, projCoords.xy).r; \n"
"    //光源视角的深度\n"
"    float currentDepth = projCoords.z;\n"
"    vec3 normal = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPosition - FragPos);\n"
"    //阴影失真\n"
"    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);\n"
"    //PCF\n"
"    float shadow = 0.0;\n"
"    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);\n"
"    for(int x = -1; x <= 1; ++x)\n"
"    {\n"
"        for(int y = -1; y <= 1; ++y)\n"
"        {\n"
"            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; \n"
"            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        \n"
"        }    \n"
"    }\n"
"    shadow /= 9.0;\n"
"    //防止采样过多\n"
"    if(projCoords.z > 1.0)\n"
"        shadow = 0.0;\n"
"        \n"
"    return shadow;\n"
"}\n"
"\n"
"void main()\n"
"{           \n"
"    vec3 color = color;\n"
"    vec3 normal = normalize(Normal);\n"
"    vec3 lightColor = vec3(0.3);\n"
"    // ambient\n"
"    vec3 ambient = 0.3 * color;\n"
"    // diffuse\n"
"    vec3 lightDir = normalize(lightPosition - FragPos);\n"
"    float diff = max(dot(lightDir, normal), 0.0);\n"
"    vec3 diffuse = diff * lightColor;\n"
"    // specular\n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    vec3 reflectDir = reflect(-lightDir, normal);\n"
"    float spec = 0.0;\n"
"    vec3 halfwayDir = normalize(lightDir + viewDir);  \n"
"    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);\n"
"    vec3 specular = spec * lightColor;    \n"
"    // calculate shadow\n"
"    float shadow = ShadowCalculation(FragPosLightSpace);                      \n"
"    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    \n"
"    \n"
"    FragColor = vec4(lighting, 1.0);\n"
"}\n";
const char* shadowMappingVertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec3 aColor;\n"
"\n"
"\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec3 color;\n"
"out vec4 FragPosLightSpace;\n"
"\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"uniform mat4 lightSpaceMatrix;\n"
"\n"
"void main()\n"
"{\n"
"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
"    Normal = transpose(inverse(mat3(model))) * aNormal;\n"
"    color = aColor;\n"
"    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\n";

//hw8
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
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
			glShaderSource(vertex, 1, &basicLightingVertexShader, NULL);
		}
		else if (a == 2) {
			glShaderSource(vertex, 1, &lampVertexShader, NULL);
		}
		else if (a == 3) {
			glShaderSource(vertex, 1, &gouraudVertexShader, NULL);
		}
		else if (a == 4) {
			glShaderSource(vertex, 1, &simpleDepthVertexShader, NULL);
		}
		else if (a == 5) {
			glShaderSource(vertex, 1, &showDepthVertexShader, NULL);
		}
		else if (a == 6) {
			glShaderSource(vertex, 1, &shadowMappingVertexShader, NULL);
		}
		else if (a == 7) {
			glShaderSource(vertex, 1, &vertexShaderSource, NULL);
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
		else if (a == 4) {
			glShaderSource(fragment, 1, &simpleDepthFragmentShader, NULL);
		}
		else if (a == 5) {
			glShaderSource(fragment, 1, &showDepthFragmentShader, NULL);
		}
		else if (a == 6) {
			glShaderSource(fragment, 1, &shadowMappingFragmentShader, NULL);
		}
		else if (a == 7) {
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
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
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