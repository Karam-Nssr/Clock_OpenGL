#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Polygon.cpp"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace glm;
int width = 1200;
int height = 1000;

vec3 cameraPos = vec3(0.0f, 0.0f, 4.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	const float cameraSpeed = 0.1f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPos += cross(cameraFront, normalize(cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos -= cross(cameraFront, normalize(cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		mat4 rotationMatrix = glm::rotate(mat4(1.0f), 0.01f, vec3(0.0f, 1.0f, 0.0f));
		vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
		cameraFront = vec3(rotatedVectorHomogeneous);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		mat4 rotationMatrix = glm::rotate(mat4(1.0f), 0.01f, vec3(0.0f, -1.0f, 0.0f));
		vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
		cameraFront = vec3(rotatedVectorHomogeneous);
	}
}

Polygon createPointer(float length, float width, vec3 color)
{
	std::vector<vec3> verts;
	verts.push_back(vec3(-width, 0.0f, 0.001f));
	verts.push_back(vec3(width, 0.0f, 0.001f));
	verts.push_back(vec3(width, length, 0.001f));
	verts.push_back(vec3(-width, length, 0.001f));

	return Polygon(verts, color);
}


void drawDigit(int digit, Shader& shader, glm::mat4 model, glm::vec3 color, float s = 0.03f)
{
	bool A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0;
	switch (digit)
	{
	case 0: A = B = C = D = E = F = 1; break;
	case 1: B = C = 1; break;
	case 2: A = B = G = E = D = 1; break;
	case 3: A = B = G = C = D = 1; break;
	case 4: F = G = B = C = 1; break;
	case 5: A = F = G = C = D = 1; break;
	case 6: A = F = G = C = D = E = 1; break;
	case 7: A = B = C = 1; break;
	case 8: A = B = C = D = E = F = G = 1; break;
	case 9: A = B = C = D = F = G = 1; break;
	default: break;
	}
	std::vector<glm::vec3> verts;
	auto pushSeg = [&](glm::vec2 a, glm::vec2 b) {
		verts.push_back(glm::vec3(a.x, a.y, 0.002f));
		verts.push_back(glm::vec3(b.x, b.y, 0.002f));
		};

	if (A) pushSeg(glm::vec2(-s, s), glm::vec2(s, s));
	if (B) pushSeg(glm::vec2(s, s), glm::vec2(s, 0));
	if (C) pushSeg(glm::vec2(s, 0), glm::vec2(s, -s));
	if (D) pushSeg(glm::vec2(-s, -s), glm::vec2(s, -s));
	if (E) pushSeg(glm::vec2(-s, 0), glm::vec2(-s, -s));
	if (F) pushSeg(glm::vec2(-s, s), glm::vec2(-s, 0));
	if (G) pushSeg(glm::vec2(-s, 0), glm::vec2(s, 0));

	if (verts.empty()) return;

	GLuint VAO = 0, VBO = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), verts.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	shader.setMat4("model", model);
	shader.setVec3("objectColor", color);

	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, (GLsizei)verts.size());

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Lecture 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");

	std::vector<vec3> circleVerts;
	float radius = 1.0f;
	int numSegments = 50;

	circleVerts.push_back(vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i <= numSegments; i++) {
		float angle = (float)i / numSegments * 2.0f * M_PI;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		circleVerts.push_back(vec3(x, y, 0.0f));
	}

	Polygon circle(circleVerts, vec3(1.0f, 1.0f, 1.0f));

	Polygon hourHand = createPointer(0.5f, 0.03f, vec3(1.0f, 0.5f, 0.0f));
	Polygon minuteHand = createPointer(0.75f, 0.02f, vec3(0.0f, 1.0f, 0.0f));
	Polygon secondHand = createPointer(0.85f, 0.01f, vec3(1.0f, 0.0f, 0.0f));

	std::vector<Polygon> hourMarks;

	for (int i = 0; i < 12; i++) {
		float angle = i * (2 * M_PI / 12);

		float x = 0.85f * cos(angle);
		float y = 0.85f * sin(angle);

		std::vector<vec3> mark = {
			vec3(-0.03f, -0.1f, 0.001f),
			vec3(0.03f, -0.1f, 0.001f),
			vec3(0.03f,  0.1f, 0.001f),
			vec3(-0.03f,  0.1f, 0.001f)
		};

		Polygon tick(mark, vec3(0.0f, 0.0f, 0.0f));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, vec3(x, y, 0.0f));
		model = glm::rotate(model, angle, vec3(0.0f, 0.0f, 1.0f));
		tick.transformation(model);

		hourMarks.push_back(tick);
	}

	struct NumberPlacement { std::string s; glm::mat4 model; };
	std::vector<NumberPlacement> numbers;

	float numberRadius = 0.75f; 

	for (int i = 1; i <= 12; ++i) {
		float angle = M_PI / 2 - i * (2.0f * M_PI / 12.0f); 
		float x = numberRadius * cos(angle);
		float y = numberRadius * sin(angle);


		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, vec3(x, y, 0.0015f)); 

		numbers.push_back({ std::to_string(i), model });
	}


	ourShader.use();

	mat4 projection = mat4(1.0f);
	projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 view = mat4(1.0f);
		view = translate(view, vec3(0.0f, 0.0f, -4.0f));
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		mat4 transformation = mat4(1.0f);

		double t = glfwGetTime();

		float secondAngle = - t * (2 * M_PI / 60);
		float minuteAngle =  - t * (2 * M_PI / 3600);
		float hourAngle = - t * (2 * M_PI / 43200);


		glm::mat4 sModel = glm::mat4(1.0f);
		sModel = glm::rotate(sModel, secondAngle, vec3(0, 0, 1));
		secondHand.transformation(sModel);

		glm::mat4 mModel = glm::mat4(1.0f);
		mModel = glm::rotate(mModel, minuteAngle, vec3(0, 0, 1));
		minuteHand.transformation(mModel);

		glm::mat4 hModel = glm::mat4(1.0f);
		hModel = glm::rotate(hModel, hourAngle, vec3(0, 0, 1));
		hourHand.transformation(hModel);

		circle.draw(ourShader);

		for (auto& tick : hourMarks)
			tick.draw(ourShader);

		hourHand.draw(ourShader);
		minuteHand.draw(ourShader);
		secondHand.draw(ourShader);

		ourShader.use();
		for (auto& np : numbers)
		{
			std::string s = np.s;
			float spacing = 0.09f; 
			int len = (int)s.size();
			for (int k = 0; k < len; ++k)
			{
				char c = s[k];
				if (c < '0' || c > '9') continue;
				int digit = c - '0';

				float offset = (k - (len - 1) / 2.0f) * spacing;
				
				glm::mat4 digitModel = np.model;
				digitModel = glm::translate(digitModel, glm::vec3(offset, 0.0f, 0.0f));
				digitModel = glm::scale(digitModel, glm::vec3(0.9f));

				drawDigit(digit, ourShader, digitModel, glm::vec3(0.0f, 0.0f, 0.0f), 0.03f);
			}
		}


		transformation = rotate(transformation, (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
