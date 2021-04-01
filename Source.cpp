#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include "Shader.h"
#include "Camera.h"

struct ModelTransform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void setScale(float s)
	{
		scale.x = s;
		scale.y = s;
		scale.z = s;
	}
};

struct Color {
	float r, g, b, a;
};

Color background = { 0.f, 0.f, 0.f, 1.f };

Camera camera(glm::vec3(0.f, 0.f, -2.f));

void OnResize(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* win, double dt)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);
	if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS)
		background = { 1.0f, 0.0f, 0.0f, 1.0f };
	if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS)
		background = { 0.0f, 1.0f, 0.0f, 1.0f };
	if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS)
		background = { 0.0f, 0.0f, 1.0f, 1.0f };

	uint32_t dir = 0;

	if (glfwGetKey(win, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		dir |= CAM_UP;
	if (glfwGetKey(win, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		dir |= CAM_DOWN;
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		dir |= CAM_FORWARD;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		dir |= CAM_BACKWARD;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		dir |= CAM_LEFT;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		dir |= CAM_RIGHT;

	double newx = 0.f, newy = 0.f;
	glfwGetCursorPos(win, &newx, &newy);
	static double x = newx, y = newy;
	double xoffset = newx - x;
	double yoffset = newy - y;
	x = newx;
	y = newy;

	camera.Move(dir, dt);
	camera.Rotate(xoffset, -yoffset);
}

void OnScroll(GLFWwindow* win, double x, double y)
{
	camera.ChangeFOV(y);
	std::cout << "Scrolled x: " << x << ", y: " << y << ". FOV = " << camera.Fov << std::endl;
}

bool wireframeMode = false;

void UpdatePolygoneMode()
{
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OnKeyAction(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE:
			wireframeMode = !wireframeMode;
			UpdatePolygoneMode();
			break;
		}
	}
}

typedef unsigned char byte;



int main()
{
#pragma region WINDOW INITIALIZATION
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(1280, 720, "OpenGL Window", NULL, NULL);
	if (win == NULL)
	{
		std::cout << "Error. Couldn't create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error. Couldn't load GLAD!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(win, OnResize);
	glfwSetScrollCallback(win, OnScroll);
	glfwSetKeyCallback(win, OnKeyAction);

	glViewport(0, 0, 1280, 720);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	UpdatePolygoneMode();
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

#pragma endregion

	int box_width, box_height, channels;
	byte* data = stbi_load("images\\box.png", &box_width, &box_height, &channels, 0);


	const int verts = 36;

	float cube[] = {
		//position			normal					texture				color			
	-1.0f,-1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f,-1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,

	1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,

	-1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,

	1.0f, 1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	1.0f,-1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	1.0f,-1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	1.0f,-1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f
	};

	ModelTransform polygonTrans1 = { glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(1.f, 1.f, 1.f) };	// scale

	ModelTransform polygonTrans2 = { glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(1.f, 1.f, 1.f) };	// scale

	ModelTransform polygonTrans3 = { glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(1.f, 1.f, 1.f) };	// scale

	ModelTransform lightTrans = {   glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(0.1f, 0.1f, 0.1f) };	// scale

#pragma region BUFFERS INITIALIZATION
	unsigned int box_texture;
	glGenTextures(1, &box_texture);

	glBindTexture(GL_TEXTURE_2D, box_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB,  GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	unsigned int VBO_polygon, VAO_polygon;
	glGenBuffers(1, &VBO_polygon);
	glGenVertexArrays(1, &VAO_polygon);

	glBindVertexArray(VAO_polygon);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

#pragma endregion

	Shader* polygon_shader = new Shader("shaders\\basic.vert", "shaders\\basic.frag");
	Shader* light_shader = new Shader("shaders\\light.vert", "shaders\\light.frag");

	double oldTime = glfwGetTime(), newTime, deltaTime;

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);


	while (!glfwWindowShouldClose(win))
	{
		newTime = glfwGetTime();
		deltaTime = newTime - oldTime;
		oldTime = newTime;

		processInput(win, deltaTime);

		polygonTrans1.rotation.z = glfwGetTime() * 60.0;
		//polygonTrans1.rotation.x = glfwGetTime() * 45.0;
		polygonTrans1.position.x = 0.6f*cos(glfwGetTime()*0.5);
		polygonTrans1.position.y = 0.6f*sin(glfwGetTime()*0.5);
		polygonTrans1.setScale(0.2f);
		
		
		polygonTrans2.rotation.z = glfwGetTime() * 30.0;
		//polygonTrans2.rotation.y = glfwGetTime() * 45.0;
		polygonTrans2.position.x = 0.6f * cos(glfwGetTime()*0.5 + 3.14158f);
		polygonTrans2.position.y = 0.6f * sin(glfwGetTime()*0.5 + 3.14158f);
		polygonTrans2.setScale(0.2f);

		polygonTrans3.setScale(0.2f);
		//polygonTrans3.rotation.x = glfwGetTime() * 90.0;
		//polygonTrans3.rotation.y = glfwGetTime() * 60.0;

		lightPos.x = 2.0f * cos(glfwGetTime() * 1.2f);
		lightPos.y = 0.0f;
		lightPos.z = 2.0f * sin(glfwGetTime() * 1.2f);
		lightTrans.position = lightPos;

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		//camera.Rotate(0.5f, 0);

		glm::mat4 p = camera.GetProjectionMatrix();
		glm::mat4 v = camera.GetViewMatrix();
		glm::mat4 pv = p * v;

		// 1
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, polygonTrans1.position);
		model = glm::rotate(model, glm::radians(polygonTrans1.rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(polygonTrans1.rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(polygonTrans1.rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, polygonTrans1.scale);


		polygon_shader->use();
		polygon_shader->setMatrix4F("pv", pv);
		polygon_shader->setMatrix4F("model", model);
		polygon_shader->setBool("wireframeMode", wireframeMode);
		polygon_shader->setVec3("viewPos", camera.Position);
		polygon_shader->setVec3("lightPos", lightPos);
		polygon_shader->setVec3("lightColor", lightColor);
		polygon_shader->setVec3("ambientColor", ambientColor);


		glBindTexture(GL_TEXTURE_2D, box_texture);
		glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, polygonTrans2.position);
		model = glm::rotate(model, glm::radians(polygonTrans2.rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(polygonTrans2.rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(polygonTrans2.rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, polygonTrans2.scale);

		polygon_shader->use();
		polygon_shader->setMatrix4F("pv", pv);
		polygon_shader->setMatrix4F("model", model);
		polygon_shader->setBool("wireframeMode", wireframeMode);
		polygon_shader->setVec3("viewPos", camera.Position);
		polygon_shader->setVec3("lightPos", lightPos);
		polygon_shader->setVec3("lightColor", lightColor);
		polygon_shader->setVec3("ambientColor", ambientColor);

		//glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, polygonTrans3.position);
		model = glm::rotate(model, glm::radians(polygonTrans3.rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(polygonTrans3.rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(polygonTrans3.rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, polygonTrans3.scale);

		polygon_shader->use();
		polygon_shader->setMatrix4F("pv", pv);
		polygon_shader->setMatrix4F("model", model);
		polygon_shader->setBool("wireframeMode", wireframeMode);
		polygon_shader->setVec3("viewPos", camera.Position);
		polygon_shader->setVec3("lightPos", lightPos);
		polygon_shader->setVec3("lightColor", lightColor);
		polygon_shader->setVec3("ambientColor", ambientColor);

		glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// LIGHT
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightTrans.position);
		model = glm::scale(model, lightTrans.scale);

		light_shader->use();
		light_shader->setMatrix4F("pv", pv);
		light_shader->setMatrix4F("model", model);
		light_shader->setVec3("lightColor", lightColor);

		glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	delete polygon_shader;

	glfwTerminate();
	return 0;
}