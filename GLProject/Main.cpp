
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
int width = 800;
int height = 600;

vec3 cameraPos = vec3(0.0f, 0.0f, 4.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.9f; 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPos += cross(cameraFront,normalize(cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos -= cross(cameraFront, normalize(cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		mat4 rotationMatrix = glm::rotate(mat4(1.0f), 0.001f, vec3(0.0f, 1.0f, 0.0f));
		vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
		cameraFront = vec3(rotatedVectorHomogeneous);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		mat4 rotationMatrix = glm::rotate(mat4(1.0f), 0.001f, vec3(0.0f, -1.0f, 0.0f));
		vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
		cameraFront = vec3(rotatedVectorHomogeneous);
	}
}


int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Lecture 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");

	std::vector<vec3> vertices0 = {};
	vertices0.push_back(vec3(0.01f, -3.0f, 0.0f));
	vertices0.push_back(vec3(-0.01f, -3.0f, 0.0f));
	vertices0.push_back(vec3(-0.01f, 3.0f, 0.0f));
	vertices0.push_back(vec3(0.01f, 3.0f, 0.0f));
	Polygon axis = Polygon(vertices0, vec3(0.0f, 0.0f, 0.0f));

	std::vector<vec3> vertices1 = {};
	vertices1.push_back(vec3(0.5f, -0.5f, 0.0f));
	vertices1.push_back(vec3(-0.5f, -0.5f, 0.0f));
	vertices1.push_back(vec3(0.0f, 0.366f, 0.0f));
	Polygon Polygon1 = Polygon(vertices1, vec3(1.0f, 0.0f, 0.0f));

	std::vector<vec3> vertices2 = {};
	vertices2.push_back(vec3(0.5f, -0.5f, 1.0f));
	vertices2.push_back(vec3(-0.5f, -0.5f, 1.0f));
	vertices2.push_back(vec3(0.0f, 0.366f, 1.0f));
	Polygon Polygon2 = Polygon(vertices2, vec3(0.0f, 1.0f, 0.0f));

	std::vector<vec3> vertices3 = {};
	vertices3.push_back(vec3(0.0f, 0.366f, 0.0f));
	vertices3.push_back(vec3(0.0f, 0.366f, 1.0f));
	vertices3.push_back(vec3(0.5f, -0.5f, 0.0f));
	vertices3.push_back(vec3(0.5f, -0.5f, 1.0f));
	vertices3.push_back(vec3(0.0f, 0.366f, 1.0f));
	Polygon Polygon3 = Polygon(vertices3, vec3(0.0f, 0.0f, 1.0f));

	std::vector<vec3> vertices4 = {};
	vertices4.push_back(vec3(0.0f, 0.366f, 0.0f));
	vertices4.push_back(vec3(0.0f, 0.366f, 1.0f));
	vertices4.push_back(vec3(-0.5f, -0.5f, 0.0f));
	vertices4.push_back(vec3(-0.5f, -0.5f, 1.0f));
	vertices4.push_back(vec3(0.0f, 0.366f, 1.0f));
	Polygon Polygon4 = Polygon(vertices4, vec3(1.0f, 1.0f, 0.0f));

	std::vector<vec3> vertices5 = {};
	vertices5.push_back(vec3(0.0f, 0.5f, -0.5f));
	vertices5.push_back(vec3(0.0f, 0.5f, 0.5f));
	vertices5.push_back(vec3(-0.5f, -0.5f, 0.0f));
	Polygon Polygon5 = Polygon(vertices5, vec3(1.0f, 1.0f, 0.0f));

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

	Polygon circle(circleVerts, vec3(0.0f, 0.8f, 1.0f));  


	ourShader.use();
	
	mat4 projection = mat4(1.0f);
	projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	//projection = ortho(-(float)width, (float)width, -(float)height, (float)height, 0.01f, 100.0f);;
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
		//transformation = translate(transformation, vec3(-0.5f, -0.5f, 0.0f));
		//axis.transformation(transformation);
		axis.draw(ourShader);
		circle.draw(ourShader);

		transformation = rotate(transformation, (float)glfwGetTime(), vec3(1.0f, 0.0f, 0.0f));
		//transformation = translate(transformation, vec3(0.5f, 0.5f, 0.0f));
		
		
		//Polygon1.transformation(transformation);
		//Polygon1.draw(ourShader);

		//Polygon2.transformation(transformation);
		//Polygon2.draw(ourShader);

		//Polygon3.transformation(transformation);
		//Polygon3.draw(ourShader);

		//Polygon4.transformation(transformation);
		//Polygon4.draw(ourShader);

		//Polygon5.transformation(transformation);
		//Polygon5.draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}