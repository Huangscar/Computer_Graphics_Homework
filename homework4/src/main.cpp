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


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace std;



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
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

	//初始化imgui
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
	//io.Fonts->AddFontDefault();



	//初始化
	float point1[3] = { 0.0f, 0.0f, 0.0f };
	float point2[3] = { 0.5f, 0.5f, 0.0f };
	float point3[3] = { 0.5f, -0.5f, 0.0f };
	bool getPoint = false;
	bool getCircle = false;
	bool getColor = false;
	bool finishColor = false;
	bool finishChangeColor = true;
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	int status = 0;
	float vertices[9000];
	unsigned int VBO, VAO;
	int radius = 1;
	vector<float> vertice;
	ImVec4 chooseColor = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	int hwNum = 3;
	bool showQuestion = true;
	Shader shader;
	float vertices3[] = {
		-2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		 2.0f, -2.0f, -2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		 2.0f,  2.0f, -2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,

		-2.0f, -2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		 2.0f, -2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,

		-2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		-2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		-2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,

		 2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		 2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,

		-2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		 2.0f, -2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		 2.0f, -2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,

		-2.0f,  2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		 2.0f,  2.0f, -2.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f,  2.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		-2.0f,  2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f
	};
	
	bool threeIsDraw = false;
	int threeNum = 1;
	bool deepTest = true;
	float R = 2.0f;//球的半径
	float pi = 3.14159;
	int statck = 502;//statck：切片----把球体横向切成几部分
	float statckStep = (float)(pi / statck);//单位角度值
	int slice = 502;//纵向切几部分
	float sliceStep = (float)(pi / slice);//水平圆递增的角度

	float r0, r1, x0, x1, y0, y1, z0, z1; //r0、r1为圆心引向两个临近切片部分表面的两条线 (x0,y0,z0)和(x1,y1,z1)为临近两个切面的点。
	float alpha0 = 0, alpha1 = 0; //前后两个角度
	float beta = 0; //切片平面上的角度
	float te[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
	int teNum = 0;
	vector<float> coordsList;
	//外层循环
	for (int i = 0; i < statck; i++) {
		alpha0 = (float)(-pi / 2 + (i*statckStep));
		alpha1 = (float)(-pi / 2 + ((i + 1)*statckStep));
		y0 = (float)(R * sin(alpha0));
		r0 = (float)(R * cos(alpha0));
		y1 = (float)(R * sin(alpha1));
		r1 = (float)(R * cos(alpha1));

		//循环每一层圆
		for (int j = 0; j <= (slice * 2); j++) {
			beta = j * sliceStep;
			x0 = (float)(r0 * cos(beta));
			z0 = -(float)(r0 * sin(beta));
			x1 = (float)(r1 * cos(beta));
			z1 = -(float)(r1 * sin(beta));
			coordsList.push_back(x0);
			coordsList.push_back(y0);
			coordsList.push_back(z0);
			coordsList.push_back(te[teNum]);
			teNum++;
			coordsList.push_back(te[teNum]);
			teNum++;
			if (teNum == 12) {
				teNum = 0;
			}
			coordsList.push_back(x1);
			coordsList.push_back(y1);
			coordsList.push_back(z1);
			coordsList.push_back(te[teNum]);
			teNum++;
			coordsList.push_back(te[teNum]);
			teNum++;
			if (teNum == 12) {
				teNum = 0;
			}
			
		}
	}
	vector<float> ballV;
	
	glm::vec3 ballPosition[] = {
		glm::vec3(8.0f, 0.0f, 0.0f),
		glm::vec3(8.0f, 0.0f, 0.0f),
		glm::vec3(8.0f, 0.0f, 0.0f)

	};


	//创建着色器
	int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//编译着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader);

	//检测编译错误
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

	//创建着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//链接着色器
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//判断是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR:SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
	}
	glUseProgram(shaderProgram);
	//删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); 
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



	//准备引擎
	while (!glfwWindowShouldClose(window)) {

		//创建ImGui
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("choose homework");

		if (ImGui::RadioButton("homework 2", hwNum == 2)) {
			hwNum = 2;
		}
		if (ImGui::RadioButton("homework 3", hwNum == 3)) {
			hwNum = 3;
		}
		ImGui::End();
		if (showQuestion) {
			ImGui::Begin("choose question", &showQuestion);
			if (hwNum == 2) {
				if (ImGui::RadioButton("line or triangle", status == 0)) {
					status = 0;
					getPoint = false;
				}
				if (ImGui::RadioButton("circle", status == 1)) {
					status = 1;
					getCircle = false;
				}
				if (status == 0) {
					ImGui::InputFloat3("point1", point1);
					ImGui::InputFloat3("point2", point2);
					ImGui::InputFloat3("point3", point3);
					if (ImGui::Button("submit")) {
						ImGui::Text("%f %f\n", point1[0], point1[1]);
						getPoint = false;
					}
					if (getPoint || getColor) {
						ImGui::Checkbox("fill up with color", &getColor);
						if (getColor) {
							getPoint = false;
							ImGui::ColorEdit3("triangle color", (float*)&chooseColor);
							if (ImGui::Button("submit color")) {
								finishChangeColor = true;
								finishColor = false;
							}
						}
						else {
							finishColor = false;
							finishChangeColor = true;
						}
					}
				}
				if (status == 1) {
					ImGui::InputFloat3("center", point1);
					ImGui::InputInt("radius", &radius);
					if (ImGui::Button("submit")) {
						//ImGui::Text("%f %f\n", point1[0], point1[1]);
						getCircle = false;
					}
				}

			}
			else if (hwNum == 3) {
				if (ImGui::RadioButton("Static", threeNum == 1)) {
					threeNum = 1;
					threeIsDraw = false;
				}
				ImGui::Checkbox("deep test", &deepTest);
				if (ImGui::RadioButton("Translation", threeNum == 2)) {
					threeNum = 2;
					threeIsDraw = false;
				}
				if (ImGui::RadioButton("Rotation", threeNum == 3)) {
					threeNum = 3;
					threeIsDraw = false;
				}
				if (ImGui::RadioButton("Scaling", threeNum == 4)) {
					threeNum = 4;
					threeIsDraw = false;
				}
				if (ImGui::RadioButton("Bonus", threeNum == 5)) {
					threeNum = 5;
					threeIsDraw = false;
				}
				
			}
			ImGui::End();
		}

		if (deepTest) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}


		if (hwNum == 2) {
			//glUseProgram(shaderProgram);
			shader.setShader(3);
			shader.use();
			if (!getPoint && status == 0 && !getColor) {
				float dx = abs(point1[0] - point2[0]);
				float dy = abs(point1[1] - point2[1]);
				float plusOne = 0.005;
				bool isX = true;
				if (dx < dy) {
					isX = false;
				}
				float p = 0;
				if (isX) {
					p = 2 * dy - dx;
				}
				else {
					p = 2 * dx - dy;
				}
				num1 = dx > dy ? dx * 200 + 1 : dy * 200 + 1;
				float** vertices1 = new float*[num1];
				for (int i = 0; i < num1; i++) {
					vertices1[i] = new float[3];
					vertices1[i][0] = 0;
					vertices1[i][1] = 0;
					vertices1[i][2] = 0;
				}
				for (int i = 0; i < num1; i++) {
					if (i == 0) {
						vertices1[i][0] = point1[0];
						vertices1[i][1] = point1[1];
						vertices1[i][2] = point1[2];
					}
					else {
						if (isX) {
							if (point2[0] > vertices1[i - 1][0]) {
								vertices1[i][0] = vertices1[i - 1][0] + plusOne;
							}
							else {
								vertices1[i][0] = vertices1[i - 1][0] - plusOne;
							}
							if (p > 0) {
								if (point2[1] > vertices1[i - 1][1]) {
									vertices1[i][1] = vertices1[i - 1][1] + plusOne;
								}
								else {
									vertices1[i][1] = vertices1[i - 1][1] - plusOne;
								}
								p += 2 * dy - 2 * dx;
							}
							else {
								vertices1[i][1] = vertices1[i - 1][1];
								p += 2 * dy;
							}
						}
						else {
							if (point2[1] > vertices1[i - 1][1]) {
								vertices1[i][1] = vertices1[i - 1][1] + plusOne;
							}
							else {
								vertices1[i][1] = vertices1[i - 1][1] - plusOne;
							}
							if (p > 0) {
								if (point2[0] > vertices1[i - 1][0]) {
									vertices1[i][0] = vertices1[i - 1][0] + plusOne;
								}
								else {
									vertices1[i][0] = vertices1[i - 1][0] - plusOne;
								}
								p += 2 * dx - 2 * dy;
							}
							else {
								vertices1[i][0] = vertices1[i - 1][0];
								p += 2 * dx;
							}
						}
					}
				}
				dx = abs(point1[0] - point3[0]);
				dy = abs(point1[1] - point3[1]);
				//float plusOne = 0.005;
				isX = true;
				if (dx < dy) {
					isX = false;
				}
				p = 0;
				if (isX) {
					p = 2 * dy - dx;
				}
				else {
					p = 2 * dx - dy;
				}
				num2 = dx > dy ? dx * 200 + 1 : dy * 200 + 1;
				float** vertices2 = new float*[num2];
				for (int i = 0; i < num2; i++) {
					vertices2[i] = new float[3];
					vertices2[i][0] = 0;
					vertices2[i][1] = 0;
					vertices2[i][2] = 0;
				}
				for (int i = 0; i < num2; i++) {
					if (i == 0) {
						vertices2[i][0] = point1[0];
						vertices2[i][1] = point1[1];
						vertices2[i][2] = point1[2];
					}
					else {
						if (isX) {
							if (point3[0] > vertices2[i - 1][0]) {
								vertices2[i][0] = vertices2[i - 1][0] + plusOne;
							}
							else {
								vertices2[i][0] = vertices2[i - 1][0] - plusOne;
							}
							if (p > 0) {
								if (point3[1] > vertices2[i - 1][1]) {
									vertices2[i][1] = vertices2[i - 1][1] + plusOne;
								}
								else {
									vertices2[i][1] = vertices2[i - 1][1] - plusOne;
								}
								p += 2 * dy - 2 * dx;
							}
							else {
								vertices2[i][1] = vertices2[i - 1][1];
								p += 2 * dy;
							}
						}
						else {
							if (point3[1] > vertices2[i - 1][1]) {
								vertices2[i][1] = vertices2[i - 1][1] + plusOne;
							}
							else {
								vertices2[i][1] = vertices2[i - 1][1] - plusOne;
							}
							if (p > 0) {
								if (point3[0] > vertices2[i - 1][0]) {
									vertices2[i][0] = vertices2[i - 1][0] + plusOne;
								}
								else {
									vertices2[i][0] = vertices2[i - 1][0] - plusOne;
								}
								p += 2 * dx - 2 * dy;
							}
							else {
								vertices2[i][0] = vertices2[i - 1][0];
								p += 2 * dx;
							}
						}
					}
				}
				dx = abs(point2[0] - point3[0]);
				dy = abs(point2[1] - point3[1]);
				//float plusOne = 0.005;
				isX = true;
				if (dx < dy) {
					isX = false;
				}
				p = 0;
				if (isX) {
					p = 2 * dy - dx;
				}
				else {
					p = 2 * dx - dy;
				}
				num3 = dx > dy ? dx * 200 + 1 : dy * 200 + 1;
				float** vertices3 = new float*[num3];
				for (int i = 0; i < num3; i++) {
					vertices3[i] = new float[3];
					vertices3[i][0] = 0;
					vertices3[i][1] = 0;
					vertices3[i][2] = 0;
				}
				for (int i = 0; i < num3; i++) {
					if (i == 0) {
						vertices3[i][0] = point2[0];
						vertices3[i][1] = point2[1];
						vertices3[i][2] = point2[2];
					}
					else {
						if (isX) {
							if (point3[0] > vertices3[i - 1][0]) {
								vertices3[i][0] = vertices3[i - 1][0] + plusOne;
							}
							else {
								vertices3[i][0] = vertices3[i - 1][0] - plusOne;
							}
							if (p > 0) {
								if (point3[1] > vertices3[i - 1][1]) {
									vertices3[i][1] = vertices3[i - 1][1] + plusOne;
								}
								else {
									vertices3[i][1] = vertices3[i - 1][1] - plusOne;
								}
								p += 2 * dy - 2 * dx;
							}
							else {
								vertices3[i][1] = vertices3[i - 1][1];
								p += 2 * dy;
							}
						}
						else {
							if (point3[1] > vertices3[i - 1][1]) {
								vertices3[i][1] = vertices3[i - 1][1] + plusOne;
							}
							else {
								vertices3[i][1] = vertices3[i - 1][1] - plusOne;
							}
							if (p > 0) {
								if (point3[0] > vertices3[i - 1][0]) {
									vertices3[i][0] = vertices3[i - 1][0] + plusOne;
								}
								else {
									vertices3[i][0] = vertices3[i - 1][0] - plusOne;
								}
								p += 2 * dx - 2 * dy;
							}
							else {
								vertices3[i][0] = vertices3[i - 1][0];
								p += 2 * dx;
							}
						}
					}
				}
				//float* vertices = new float[num1 * 3];
				for (int i = 0; i < (num1 + num2 + num3) * 3; i++) {
					vertices[i] = 0;
				}
				for (int i = 0; i < num1; i++) {
					//cout << vertices1[i][0] << " " << vertices1[i][1] << " " << vertices1[i][2] << endl;
					vertices[i * 3] = vertices1[i][0];
					vertices[i * 3 + 1] = vertices1[i][1];
					vertices[i * 3 + 2] = vertices1[i][2];
				}
				for (int i = num1; i < num1 + num2; i++) {
					vertices[i * 3] = vertices2[i - num1][0];
					vertices[i * 3 + 1] = vertices2[i - num1][1];
					vertices[i * 3 + 2] = vertices2[i - num1][2];
				}
				for (int i = num1 + num2; i < num1 + num2 + num3; i++) {
					vertices[i * 3] = vertices3[i - num1 - num2][0];
					vertices[i * 3 + 1] = vertices3[i - num1 - num2][1];
					vertices[i * 3 + 2] = vertices3[i - num1 - num2][2];
				}

				//Bresenham画线
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices
					, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
				getPoint = true;
			}

			if (!getCircle && status == 1) {
				vertice.clear();
				float r = 2.0f / 800 * radius;
				float firstPoint[] = {
					0, r, 0.0f
				};
				int i = 0;
				float startX = 0;
				float startY = r;
				float plusOne = 0.005;
				float p = 3 * plusOne - 2 * r;
				while (startX <= startY) {
					if (vertice.size() == 0) {
						vertice.push_back(point1[0] + 0.0f);
						vertice.push_back(point1[1] + r);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + r);
						vertice.push_back(point1[1] + 0.0f);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + 0.0f);
						vertice.push_back(point1[1] - r);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] - r);
						vertice.push_back(point1[1] + 0.0f);
						vertice.push_back(0.0f);

					}
					else {
						if (p < 0) {

							p = p + 4 * startX + 6 * plusOne;
							startX += plusOne;

						}
						else {

							p = p + 4 * (startX - startY) + 10 * plusOne;
							startX += plusOne;
							startY -= plusOne;

						}
						vertice.push_back(point1[0] + startX);
						vertice.push_back(point1[1] + startY);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + startY);
						vertice.push_back(point1[1] + startX);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + -startX);
						vertice.push_back(point1[1] + startY);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + -startY);
						vertice.push_back(point1[1] + startX);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + startX);
						vertice.push_back(point1[1] + -startY);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + startY);
						vertice.push_back(point1[1] + -startX);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + -startX);
						vertice.push_back(point1[1] + -startY);
						vertice.push_back(0.0f);
						vertice.push_back(point1[0] + -startY);
						vertice.push_back(point1[1] + -startX);
						vertice.push_back(0.0f);
					}
				}
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(float), vertice.data()
					, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
				getCircle = true;

			}

			if (!finishColor && getColor && status == 0 && finishChangeColor) {
				vertice.clear();
				float plusOne = 0.005;
				vector<float> fourPoint = getMinAndMax(point1, point2, point3);
				float x = fourPoint[0], y = fourPoint[1], maxX = fourPoint[2], maxY = fourPoint[3];
				while (x < maxX) {
					while (y < maxY) {
						float PA[] = {
							-x + point1[0], -y + point1[1], 0.0f
						};
						float PB[] = {
							-x + point2[0], -y + point2[1], 0.0f
						};
						float PC[] = {
							-x + point3[0], -y + point3[1], 0.0f
						};
						float t1 = PA[0] * PB[1] - PA[1] * PB[0];
						float t2 = PB[0] * PC[1] - PB[1] * PC[0];
						float t3 = PC[0] * PA[1] - PC[1] * PA[0];

						if (t1*t2 >= 0 && t1*t3 >= 0) {
							vertice.push_back(x);
							vertice.push_back(y);
							vertice.push_back(0.0f);
							vertice.push_back(chooseColor.x);
							vertice.push_back(chooseColor.y);
							vertice.push_back(chooseColor.z);
						}
						y += plusOne;
					}
					x += plusOne;
					y = fourPoint[1];
				}

				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(float), vertice.data()
					, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
				finishColor = true;
				finishChangeColor = false;
			}
		}
		else if (hwNum == 3 && !threeIsDraw && threeNum != 5) {
			shader.setShader(1);
			shader.use();
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			threeIsDraw = true;
		}
		else if (hwNum == 3 && !threeIsDraw && threeNum == 5) {
			shader.setShader(2);
			glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
			//shader.setInt("texture1", 0);
			shader.use();
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*coordsList.size(), coordsList.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
			glEnableVertexAttribArray(1);

			threeIsDraw = true;
		}


		//输入
		processInput(window);

		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		if (hwNum == 2) {
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (hwNum == 3) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//交换缓冲
		if (hwNum == 2) {
			if (status == 0 && getColor) {
				glUseProgram(shaderProgram);
				glBindVertexArray(VAO);
				glPointSize(5);
				glDrawArrays(GL_POINTS, 0, vertice.size() / 6);
			}
			else if (status == 0 && getPoint) {
				glUseProgram(shaderProgram);
				glBindVertexArray(VAO);
				glPointSize(5);
				glDrawArrays(GL_POINTS, 0, (num1 + num2 + num3));
			}
			else if (status == 1) {
				glUseProgram(shaderProgram);
				glBindVertexArray(VAO);
				glPointSize(5);
				glDrawArrays(GL_POINTS, 0, vertice.size() / 3);

			}
		}
		else if (hwNum == 3) {
			
			//shader.use();
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			if (threeNum != 5) {
				if (threeNum == 1) {
					model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-0.5f, 1.0f, 0.0f));
					view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
					projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				}
				else if (threeNum == 2) {
					model = glm::translate(model, glm::vec3(5 * sin(5 * glfwGetTime()), 1.0f, 0.0f));
					view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
					projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				}
				else if (threeNum == 3) {
					model = glm::rotate(model, glm::radians(50.0f*(float)glfwGetTime()), glm::vec3(1.0f, 0.0f, 1.0f));
					view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
					projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				}
				else if (threeNum == 4) {
					model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-0.5f, 1.0f, 0.0f));
					model = glm::scale(model, glm::vec3(abs(3 * sin(glfwGetTime())), abs(3 * sin(glfwGetTime())), abs(3 * sin(glfwGetTime()))));
					view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
					projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				}


				unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
				unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
				unsigned int transformLoc = glGetUniformLocation(shader.ID, "projection");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &projection[0][0]);
				//shader.setMat4("projection", projection);


				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			else {
				model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-0.5f, 1.0f, 0.0f));
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
				projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);



				unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
				unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
				unsigned int transformLoc = glGetUniformLocation(shader.ID, "projection");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &projection[0][0]);
				//shader.setMat4("projection", projection);


				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, coordsList.size()/5);

				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, glm::radians(30.0f*(float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));

				for (int i = 0; i < 3; i++) {
					if (i == 2) {
						model = glm::rotate(model, glm::radians(300.0f*(float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
					}
					model = glm::translate(model, ballPosition[i]);
					glm::mat4 model2 = glm::mat4(1.0f);
					model2 = glm::rotate(model, glm::radians(300.0f*(float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
					//shader.setMat4("model", model2);
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model2[0][0]);

				
					glDrawArrays(GL_TRIANGLE_STRIP, 0, coordsList.size() / 5);
				}
			}
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//释放资源
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

