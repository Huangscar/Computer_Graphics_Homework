#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace std;

//������ɫ��
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

//Ƭ����ɫ��
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


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



int main()
{
	//ʵ��������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "LernOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Fail to initialize GLAD" << endl;
		return -1;
	}

	//��Ⱦ���ڵ�ά��
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��ʼ��imgui
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
	//io.Fonts->AddFontDefault();

	

	//��ʼ��
	ImVec4 rightColor = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImVec4 topColor = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
	ImVec4 leftColor = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	ImVec4 choseColor = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	bool ImGui = true;
	bool simpleTriangle = true;
	bool hasChooseColor = false;
	bool eboChoose = false;
	bool hasLine = false;
	unsigned int VBO, VAO, EBO;

	

	//׼������
	while (!glfwWindowShouldClose(window)) {

		//����ImGui
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("choose color");
		ImGui::ColorEdit3("triangle color", (float*)&choseColor);
		ImGui::Checkbox("change color", &hasChooseColor);
		ImGui::Checkbox("simple triangle", &simpleTriangle);
		ImGui::Checkbox("EBO", &eboChoose);
		ImGui::Checkbox("show line", &hasLine);
		ImGui::End();

		//������ɫ��
		int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//������ɫ��
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		if (eboChoose || simpleTriangle || hasLine) {
			glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
		}
		else {
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		}
		glCompileShader(fragmentShader);

		//���������
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		}

		//������ɫ������
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();
		//������ɫ��
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		//�ж��Ƿ�ʧ��
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			cout << "ERROR:SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
		}
		//����������
		glUseProgram(shaderProgram);
		//ɾ����ɫ������
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//������ɫ
		if (hasChooseColor) {
			topColor = choseColor;
			leftColor = choseColor;
			rightColor = choseColor;
			choseColor = choseColor;
		}
		else {
			rightColor = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
			topColor = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
			leftColor = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
			choseColor = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
		}

		//��������������
		float vertices1[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		float vertices2[] = {
			-0.5f, -0.5f, 0.0f, topColor.x, topColor.y, topColor.z,
			0.5f, -0.5f, 0.0f, leftColor.x, leftColor.y, leftColor.z,
			0.0f, 0.5f, 0.0f, rightColor.x, rightColor.y, rightColor.z,
		};
		float vertices3[]{
			-1.0f, -0.5f, 0.0f,
			0.0f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			1.0f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f
		};
		unsigned int indices[] = {
			0, 1, 2,
			1, 3, 4
		};
		float vertices4[] = {
			-0.5f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f
		};


		//����VBO����,�����뻺��
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//��
		glBindVertexArray(VAO);
		//���Ƶ�������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (eboChoose) {
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
			//��ʼ��
			glGenBuffers(1, &EBO);
			//�󶨻���
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}
		else {
			if (simpleTriangle) {
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
			}
			else if (hasLine) {
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
			}
		}
		//���Ӷ�������
		if (eboChoose) {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		else {
			if (simpleTriangle) {
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			else if (hasLine) {
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			else {
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
			}
		}

		//����
		processInput(window);

		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��ͼ
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		if (eboChoose) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else if (hasLine) {
			glDrawArrays(GL_LINE_STRIP, 0, 2);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		//��������
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//�ͷ���Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

