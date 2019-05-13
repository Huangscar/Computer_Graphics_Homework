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


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
using namespace std;

float curr = 0.0f;
float delt = 0.0f;
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
Camera camera;

unsigned int planeVAO, planeVBO;

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

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
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
}

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
	glfwSetCursorPosCallback(window, mouseCallBack);

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
	unsigned int VBO, cubeVAO;
	Shader showDepthShader, depthShader, shadowMappingShader;
	shadowMappingShader.setShader(6);
	depthShader.setShader(4);
	showDepthShader.setShader(5);
	float vertices3[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,  0.2f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,  0.2f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,  0.2f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.2f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.2f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.2f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.2f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.5f,  0.2f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.5f,  0.2f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.5f,  0.2f
	};

	//平面绘制
	float planeVertices[] = {
		// 位置                 // 法向量          // 纹理
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.5f,  0.2f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.5f,  0.2f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.5f,  0.2f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.5f,  0.2f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.5f,  0.2f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.5f,  0.2f
	};

	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	//深度贴图
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// 创建2D纹理，给帧缓冲的深度缓冲使用
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// 深度纹理作为帧缓冲的深度缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowMappingShader.setShader(6);
	shadowMappingShader.use();
	shadowMappingShader.setInt("diffuseTexture", 0);
	shadowMappingShader.setInt("shadowMap", 1);
	showDepthShader.setShader(5);
	showDepthShader.use();
	showDepthShader.setInt("fboAttachment", 0);

	bool IsDraw = false;
	int questionNum = 1;

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
		if (ImGui::RadioButton("Orthographic", questionNum == 1)) {
			IsDraw = false;
			questionNum = 1;
		}
		if (ImGui::RadioButton("Projection", questionNum == 2)) {
			IsDraw = false;
			questionNum = 2;
		}
		ImGui::Checkbox("bonus", &isBonus);

		ImGui::End();

		glEnable(GL_DEPTH_TEST);

		if (!IsDraw) {

			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(cubeVAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			IsDraw = true;
		}

		//输入
		float now = glfwGetTime();
		delt = now - curr;
		curr = now;
		processInput(window);

		//渲染
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//光源空间的变换
		glm::mat4 lightProjection, lightView, lightSpaceMatrix;
		float nearPlane = 1.0f, farPlane = 7.5f;
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		if(questionNum == 1) {
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
		}
		else {
			lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, nearPlane, farPlane);
		}
		
		
		lightSpaceMatrix = lightProjection * lightView;
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//渲染深度贴图
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		//画出平面
		glm::mat4 model = glm::mat4(1.0f);
		depthShader.setMat4("model", model);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//画立方体
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		depthShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		depthShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25f));
		depthShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//重新设置viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowMappingShader.use();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shadowMappingShader.setMat4("projection", projection);
		shadowMappingShader.setMat4("view", view);
		shadowMappingShader.setVec3("lightPosition", lightPos);
		shadowMappingShader.setVec3("viewPos", camera.cameraPosition);
		shadowMappingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//画出平面
		model = glm::mat4(1.0f);
		shadowMappingShader.setMat4("model", model);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//画立方体
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shadowMappingShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shadowMappingShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25f));
		shadowMappingShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		showDepthShader.use();
		showDepthShader.setFloat("nearPlane", nearPlane);
		showDepthShader.setFloat("farPlane", farPlane);
		if (questionNum == 1) {
			showDepthShader.setFloat("type", 1);
		}
		else {
			showDepthShader.setFloat("type", 0);
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	//释放资源
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

