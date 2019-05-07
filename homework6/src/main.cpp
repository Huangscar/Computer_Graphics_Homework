#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Camera.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace std;

float curr = 0.0f;
float delt = 0.0f;
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera camera;



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.moveForward(5.0f*delt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.moveBackword(5.0f*delt);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.moveLeft(5.0f*delt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.moveRight(5.0f*delt);
	}
}

vector<float> getMinAndMax(float point1[], float point2[], float point3[]) {
	vector<float> result;
	float minX = 3.0f, minY = 3.0f, maxX = -3.0f, maxY = -3.0f;
	if (minX > point1[0]) {
		minX = point1[0];
	}
	if (minX > point2[0]) {
		minX = point2[0];
	}
	if (minX > point3[0]) {
		minX = point3[0];
	}
	if (maxX < point1[0]) {
		maxX = point1[0];
	}
	if (maxX < point2[0]) {
		maxX = point2[0];
	}
	if (maxX < point3[0]) {
		maxX = point3[0];
	}
	if (minY > point1[1]) {
		minY = point1[1];
	}
	if (minY > point2[1]) {
		minY = point2[1];
	}
	if (minY > point3[1]) {
		minY = point3[1];
	}
	if (maxY < point1[1]) {
		maxY = point1[1];
	}
	if (maxY < point2[1]) {
		maxY = point2[1];
	}
	if (maxY < point3[1]) {
		maxY = point3[1];
	}
	result.push_back(minX);
	result.push_back(minY);
	result.push_back(maxX);
	result.push_back(maxY);
	return result;
}

float dotMinus(float v1[], float v2[]) {
	return v1[0] * v2[0] + v1[1] * v2[1];
}

/*void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xOffset = xpos - lastX;
	float yOffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	camera.rotate(xOffset*0.5f, yOffset*0.5f);
}*/



int main()
{
	//实例化窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 800, "LernOpenGL", NULL, NULL);
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



	//渲染窗口的维度
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//加载鼠标控制
	//glfwSetCursorPosCallback(window, mouseCallBack);

	//初始化imgui
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
	//io.Fonts->AddFontDefault();

	//初始化
	unsigned int VBO, cubeVAO, lightVAO;
	Shader lightingShader, lampShader;
	float vertices3[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	bool IsDraw = false;
	int questionNum = 1;
	float left = -400.0f;
	float right = 400.0f;
	float bottom = -300.0f;
	float top = 300.0f;
	float nearInput = -1000.0f;
	float farInput = 1000.0f;
	float frovy = glm::radians(45.0f);
	float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	float zNear = 0.1f;
	float zFar = 100.0f;
	float materialAmbientFloat[] = { 1.0f, 0.5f, 0.31f };
	float materialDiffuseFloat[] = { 1.0f, 0.5f, 0.31f };
	float materialSpecularFloat[] = { 0.5f, 0.5f, 0.5f };
	float materialShininess = 32.0f;
	bool isBonus = false;
	//glm::perspective()

	//准备引擎
	while (!glfwWindowShouldClose(window)) {

		//创建ImGui
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("choose homework");
		if (ImGui::RadioButton("Phong Shading", questionNum == 1)) {
			IsDraw = false;
			questionNum = 1;
		}
		if (ImGui::RadioButton("Gouraud Shading", questionNum == 2)) {
			IsDraw = false;
			questionNum = 2;
		}
		ImGui::Checkbox("bonus", &isBonus);
		
		if (questionNum == 1 && !IsDraw) {
			lightingShader.setShader(1);
			lampShader.setShader(2);
		}
		if (questionNum == 2 && !IsDraw) {
			lightingShader.setShader(3);
			lampShader.setShader(2);
		}
		if (questionNum == 1) {
			ImGui::InputFloat3("material ambient", materialAmbientFloat);
			ImGui::InputFloat3("material diffuse", materialDiffuseFloat);
			ImGui::InputFloat3("material specular", materialSpecularFloat);
			ImGui::InputFloat("material shininess", &materialShininess);
		}
		if (questionNum == 2) {
			ImGui::InputFloat3("material ambient", materialAmbientFloat);
			ImGui::InputFloat3("material diffuse", materialDiffuseFloat);
			ImGui::InputFloat3("material specular", materialSpecularFloat);
			ImGui::InputFloat("material shininess", &materialShininess);
		}
		ImGui::End();

		glEnable(GL_DEPTH_TEST);

		if (!IsDraw) {

			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(cubeVAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glGenVertexArrays(1, &lightVAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(lightVAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);



			IsDraw = true;
		}

		//输入
		float now = glfwGetTime();
		delt = now - curr;
		curr = now;
		processInput(window);

		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (isBonus) {
			lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
			lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		}
		else {
			lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
		}

		//传入位置方向参数
		lightingShader.use();
		lightingShader.setVec3("lightPosition", lightPos);
		lightingShader.setVec3("viewPos", camera.cameraPosition);
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);

		//光源光照参数
		glm::vec3 lightColor;
		lightColor.x = 1.0f;
		lightColor.y = 1.0f;
		lightColor.z = 1.0f;
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
		lightingShader.setVec3("lightAmbient", ambientColor);
		lightingShader.setVec3("lightDiffuse", diffuseColor);
		lightingShader.setVec3("lightSpecular", 1.0f, 1.0f, 1.0f);

		//物体光照参数
		lightingShader.setVec3("materialAmbient", materialAmbientFloat[0], materialAmbientFloat[1], materialAmbientFloat[2]);
		lightingShader.setVec3("materialDiffuse", materialDiffuseFloat[0], materialDiffuseFloat[1], materialDiffuseFloat[2]);
		lightingShader.setVec3("materialSpecular", materialSpecularFloat[0], materialSpecularFloat[1], materialSpecularFloat[2]);
		lightingShader.setFloat("materialShininess", materialShininess);

		//交换缓冲
		//shader.use();
		float Radius = 10.0f;
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		lightingShader.setMat4("model", model);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	//释放资源
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

