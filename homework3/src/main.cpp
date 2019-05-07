#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <iostream>
#include <math.h>
#include <vector>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace std;

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

	//创建着色器
	int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//编译着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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
	//激活程序对象
	glUseProgram(shaderProgram);
	//删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	//准备引擎
	while (!glfwWindowShouldClose(window)) {

		//创建ImGui
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("choose color");
		if(ImGui::RadioButton("line or triangle", status == 0)) {
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
		ImGui::End();

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
					
					if (t1*t2>=0&&t1*t3>=0) {
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

		//输入
		processInput(window);

		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//交换缓冲
		if (status == 0 && getColor) {
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, vertice.size()/6);
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
			glDrawArrays(GL_POINTS, 0, vertice.size()/3);
			
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

