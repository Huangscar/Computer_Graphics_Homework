#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "shader.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
using namespace std;

float curr = 0.0f;
float delt = 0.0f;
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
double startX = 0.0f, startY = 0.0f;
vector<unsigned int> lineVAO, lineVBO;
unsigned int bezierVAO, bezierVBO;
unsigned int showVAO, showVBO;
//unsigned int lineVAO, lineVBO;
vector<glm::vec3> pos;
bool isShow = false;
double currTime;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (!isShow) {
			isShow = true;
			currTime = glfwGetTime();
		}
	}
	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
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
	}*/
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

float getCombinatorialNumber(int n, int m) {
	if (m == 0 || n == m) {
		return 1.0f;
	}
	else {
		float up = 1.0f, down = 1.0f;
		for (int i = 0; i < n - m; i++) {
			up *= (n - i);
			down *= (i + 1);
		}
		return up / down;
	}
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			
			if (!isShow) {
				double nowPosX, nowPosY;
				glfwGetCursorPos(window, &nowPosX, &nowPosY);
				nowPosX = (-1) + (nowPosX / SCR_WIDTH) * 2;
				nowPosY = (-1)*((-1) + (nowPosY / SCR_HEIGHT) * 2);
				unsigned int VAO, VBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				lineVAO.push_back(VAO);
				lineVBO.push_back(VBO);
				if (firstMouse) {
					//printf("button");
					double verticesLine[] = {
						nowPosX, nowPosY, 0.0f,
						nowPosX, nowPosY, 0.0f
					};
					//printf("%f %f\n", windowPosX, windowPosY);
					glBindBuffer(GL_ARRAY_BUFFER, lineVBO[lineVBO.size() - 1]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLine), verticesLine, GL_STATIC_DRAW);
					glBindVertexArray(lineVAO[lineVAO.size() - 1]);
					glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
					glEnableVertexAttribArray(0);
					//解绑
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					firstMouse = false;
				}
				else {

					double verticesLine[] = {
						startX, startY, 0.0f,
						nowPosX, nowPosY, 0.0f
					};
					//printf("%f %f\n", windowPosX, windowPosY);
					glBindBuffer(GL_ARRAY_BUFFER, lineVBO[lineVBO.size() - 1]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLine), verticesLine, GL_STATIC_DRAW);
					glBindVertexArray(lineVAO[lineVAO.size() - 1]);
					glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
					glEnableVertexAttribArray(0);
					//解绑
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}

				startX = nowPosX;
				startY = nowPosY;
				printf("%f, %f\n", startX, startY);
				firstMouse = false;

				glm::vec3 nowPos((float)startX, (float)startY, 0.0f);
				pos.push_back(nowPos);
				if (pos.size() >= 2) {
					vector<float> bezierPos;
					double t = 0;
					for (int i = 0; i <= 100; i++) {
						float x = 0.0f, y = 0.0f;
						for (int j = 0; j < pos.size(); j++) {
							float c = getCombinatorialNumber(pos.size() - 1, j);
							x += c * pow(t, j)*pow(1 - t, pos.size() - 1 - j)*pos[j].x;
							y += c * pow(t, j)*pow(1 - t, pos.size() - 1 - j)*pos[j].y;
						}
						bezierPos.push_back(x);
						bezierPos.push_back(y);
						bezierPos.push_back(0.0f);
						if (i != 0 && i != 100) {
							bezierPos.push_back(x);
							bezierPos.push_back(y);
							bezierPos.push_back(0.0f);
						}
						t += 0.01;
					}
					glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
					glBufferData(GL_ARRAY_BUFFER, bezierPos.size() * sizeof(float), bezierPos.data(), GL_STATIC_DRAW);
					glBindVertexArray(bezierVAO);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);
					//解绑
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (!isShow) {
				pos.pop_back();
				lineVAO.pop_back();
				lineVBO.pop_back();
				if (pos.size() > 0) {
					startX = pos[pos.size() - 1].x;
					startY = pos[pos.size() - 1].y;
					if (pos.size() >= 2) {
						vector<float> bezierPos;
						double t = 0;
						for (int i = 0; i <= 100; i++) {
							float x = 0.0f, y = 0.0f;
							for (int j = 0; j < pos.size(); j++) {
								float c = getCombinatorialNumber(pos.size() - 1, j);
								x += c * pow(t, j)*pow(1 - t, pos.size() - 1 - j)*pos[j].x;
								y += c * pow(t, j)*pow(1 - t, pos.size() - 1 - j)*pos[j].y;
							}
							bezierPos.push_back(x);
							bezierPos.push_back(y);
							bezierPos.push_back(0.0f);
							if (i != 0 && i != 100) {
								bezierPos.push_back(x);
								bezierPos.push_back(y);
								bezierPos.push_back(0.0f);
							}
							t += 0.01;
						}
						glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
						glBufferData(GL_ARRAY_BUFFER, bezierPos.size() * sizeof(float), bezierPos.data(), GL_STATIC_DRAW);
						glBindVertexArray(bezierVAO);
						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
						glEnableVertexAttribArray(0);
						//解绑
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindVertexArray(0);
					}
				}
				else {
					firstMouse = true;
				}
			}
			break;
		default:
			break;
		}
	}
}

int main()
{
	//实例化窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 10);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_WIDTH, "LernOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//渲染窗口的维度
	//glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallBack);

	//GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Fail to initialize GLAD" << endl;
		return -1;
	}

	Shader shader;
	shader.setShader(7);
	shader.use();

	glGenVertexArrays(1, &bezierVAO);
	glGenBuffers(1, &bezierVBO);
	glGenVertexArrays(1, &showVAO);
	glGenBuffers(1, &showVBO);
	
	//准备引擎
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		
		

		if (lineVAO.size() == 1) {
			glBindVertexArray(lineVAO[0]);
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		else if (lineVAO.size() > 1) {
			for (vector<unsigned int>::iterator iter = lineVAO.begin(); iter != lineVAO.end(); ++iter)
			{
				glBindVertexArray(*iter);
				glDrawArrays(GL_LINE_STRIP, 0, 2);
				glPointSize(5);
				glDrawArrays(GL_POINTS, 0, 2);
			}
			glBindVertexArray(bezierVAO);
			glDrawArrays(GL_LINES, 0, 200);
		}
		


		if (isShow) {
			double t = (glfwGetTime() - currTime)*0.08;
			vector<float> linePos;
			vector<glm::vec3> testPos;
			for (int i = 0; i < pos.size() - 1; i++) {
				float x = (pos[i + 1].x - pos[i].x)*t + pos[i].x;
				float y = (pos[i + 1].y - pos[i].y)*t + pos[i].y;
				linePos.push_back(x);
				linePos.push_back(y);
				linePos.push_back(0.0f);
				if (i != 0 && i != pos.size() - 2) {
					linePos.push_back(x);
					linePos.push_back(y);
					linePos.push_back(0.0f);
				}
				testPos.push_back(glm::vec3(x,y,0.0f));
			}
			while (testPos.size() > 1) {
				vector<glm::vec3> test2Pos;
				for (int i = 0; i < testPos.size() - 1; i++) {
					float x = (testPos[i + 1].x - testPos[i].x)*t + testPos[i].x;
					float y = (testPos[i + 1].y - testPos[i].y)*t + testPos[i].y;
					linePos.push_back(x);
					linePos.push_back(y);
					linePos.push_back(0.0f);
					if (i != 0 && i != testPos.size() - 2) {
						linePos.push_back(x);
						linePos.push_back(y);
						linePos.push_back(0.0f);
					}
					test2Pos.push_back(glm::vec3(x, y, 0.0f));
				}
				testPos.clear();
				testPos = test2Pos;
			}
			glBindBuffer(GL_ARRAY_BUFFER, showVBO);
			glBufferData(GL_ARRAY_BUFFER, linePos.size() * sizeof(float), linePos.data(), GL_STATIC_DRAW);
			glBindVertexArray(showVAO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			//解绑
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindVertexArray(showVAO);
			glDrawArrays(GL_LINES, 0, linePos.size()/3);
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, linePos.size() / 3);
			if (t >= 1) {
				isShow = false;
			}
		}


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	//释放资源
	//glDeleteVertexArrays(lineVAO.size(), lineVAO.data());
	//glDeleteBuffers(lineVBO.size(), lineVBO.data());
	glfwTerminate();
	return 0;
}