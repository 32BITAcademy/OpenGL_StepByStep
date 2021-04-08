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

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Light
{
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

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

	ModelTransform cubeTrans1 = { glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(1.f, 1.f, 1.f) };	// scale

	ModelTransform cubeTrans2 = { glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(1.f, 1.f, 1.f) };	// scale

	ModelTransform cubeTrans3 = { glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(1.f, 1.f, 1.f) };	// scale

	ModelTransform lightTrans = {   glm::vec3(0.f, 0.f, 0.f),	// position
									glm::vec3(0.f, 0.f, 0.f),	// rotation
									glm::vec3(0.1f, 0.1f, 0.1f) };	// scale

	Material cubeMat1 = {	glm::vec3(0.25, 0.20725, 0.20725),
							glm::vec3(1, 0.829, 0.829),
							glm::vec3(0.296648,	0.296648, 0.296648),
							12.f }; // pearl

	Material cubeMat2 = {	glm::vec3(0.25, 0.25, 0.25),
							glm::vec3(0.4, 0.4, 0.4),
							glm::vec3(0.774597,	0.774597, 0.774597),
							77.f }; // chrome
	
	Material cubeMat3 = {	glm::vec3(0.1745, 0.01175, 0.01175),
							glm::vec3(0.61424, 0.04136, 0.04136),
							glm::vec3(0.727811, 0.626959, 0.626959),
							77.f }; // ruby

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

	Light light1 = {	glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3(0.2f, 0.2f, 0.2f),
						glm::vec3(0.5f, 0.5f, 0.5f),
						glm::vec3(1.0f, 1.0f, 1.0f) };

	while (!glfwWindowShouldClose(win))
	{
		newTime = glfwGetTime();
		deltaTime = newTime - oldTime;
		oldTime = newTime;

		processInput(win, deltaTime);

		cubeTrans1.rotation.z = glfwGetTime() * 60.0;
		//cubeTrans1.rotation.x = glfwGetTime() * 45.0;
		cubeTrans1.position.x = 0.6f;
		cubeTrans1.setScale(0.2f);
		
		
		cubeTrans2.rotation.z = glfwGetTime() * 30.0;
		//cubeTrans2.rotation.y = glfwGetTime() * 45.0;
		cubeTrans2.position.x = -0.6f;
		cubeTrans2.setScale(0.2f);

		cubeTrans3.setScale(0.2f);
		//cubeTrans3.rotation.x = glfwGetTime() * 90.0;
		//cubeTrans3.rotation.y = glfwGetTime() * 60.0;

		light1.position.x = 2.0f;// *cos(glfwGetTime() * 1.2f);
		light1.position.y = 0.0f;
		light1.position.z = 2.0f; // *sin(glfwGetTime() * 1.2f);
		lightTrans.position = light1.position;

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		//camera.Rotate(0.5f, 0);

		glm::mat4 p = camera.GetProjectionMatrix();
		glm::mat4 v = camera.GetViewMatrix();
		glm::mat4 pv = p * v;

		// 1
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, cubeTrans1.position);
		model = glm::rotate(model, glm::radians(cubeTrans1.rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(cubeTrans1.rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(cubeTrans1.rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, cubeTrans1.scale);


		polygon_shader->use();
		polygon_shader->setMatrix4F("pv", pv);
		polygon_shader->setMatrix4F("model", model);
		polygon_shader->setBool("wireframeMode", wireframeMode);
		polygon_shader->setVec3("viewPos", camera.Position);
		polygon_shader->setVec3("light.position", light1.position);
		polygon_shader->setVec3("light.ambient", light1.ambient);
		polygon_shader->setVec3("light.diffuse", light1.diffuse);
		polygon_shader->setVec3("light.specular", light1.specular);
		polygon_shader->setVec3("material.ambient", cubeMat1.ambient);
		polygon_shader->setVec3("material.diffuse", cubeMat1.diffuse);
		polygon_shader->setVec3("material.specular", cubeMat1.specular);
		polygon_shader->setFloat("material.shininess", cubeMat1.shininess);


		glBindTexture(GL_TEXTURE_2D, box_texture);
		glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubeTrans2.position);
		model = glm::rotate(model, glm::radians(cubeTrans2.rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(cubeTrans2.rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(cubeTrans2.rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, cubeTrans2.scale);

		polygon_shader->use();
		polygon_shader->setMatrix4F("pv", pv);
		polygon_shader->setMatrix4F("model", model);
		polygon_shader->setBool("wireframeMode", wireframeMode);
		polygon_shader->setVec3("viewPos", camera.Position);
		polygon_shader->setVec3("light.position", light1.position);
		polygon_shader->setVec3("light.ambient", light1.ambient);
		polygon_shader->setVec3("light.diffuse", light1.diffuse);
		polygon_shader->setVec3("light.specular", light1.specular);
		polygon_shader->setVec3("material.ambient", cubeMat2.ambient);
		polygon_shader->setVec3("material.diffuse", cubeMat2.diffuse);
		polygon_shader->setVec3("material.specular", cubeMat2.specular);
		polygon_shader->setFloat("material.shininess", cubeMat2.shininess);

		//glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubeTrans3.position);
		model = glm::rotate(model, glm::radians(cubeTrans3.rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(cubeTrans3.rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(cubeTrans3.rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, cubeTrans3.scale);

		polygon_shader->use();
		polygon_shader->setMatrix4F("pv", pv);
		polygon_shader->setMatrix4F("model", model);
		polygon_shader->setBool("wireframeMode", wireframeMode);
		polygon_shader->setVec3("viewPos", camera.Position);
		polygon_shader->setVec3("light.position", light1.position);
		polygon_shader->setVec3("light.ambient", light1.ambient);
		polygon_shader->setVec3("light.diffuse", light1.diffuse);
		polygon_shader->setVec3("light.specular", light1.specular);
		polygon_shader->setVec3("material.ambient", cubeMat3.ambient);
		polygon_shader->setVec3("material.diffuse", cubeMat3.diffuse);
		polygon_shader->setVec3("material.specular", cubeMat3.specular);
		polygon_shader->setFloat("material.shininess", cubeMat3.shininess);

		glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// LIGHT
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightTrans.position);
		model = glm::scale(model, lightTrans.scale);

		light_shader->use();
		light_shader->setMatrix4F("pv", pv);
		light_shader->setMatrix4F("model", model);
		light_shader->setVec3("lightColor", light1.specular);

		glBindVertexArray(VAO_polygon);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	delete polygon_shader;

	glfwTerminate();
	return 0;
}