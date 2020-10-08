#pragma once
#include "Input.h"
#include "Matrix.h"
#include <iostream>
#include <vector>

# define M_PI           3.14159265358979323846  /* pi */

extern int width, height;
extern std::vector<Vertex> pointsCloud;

extern Color choosedColor;
extern float step;
extern bool movingPoint;

extern int selectedPointId;


bool wantToMovePoint;
bool enable3DViewport;
extern bool enableWireframe;
extern bool enableNormal;

Input::Input()
{
	ViewMatrix = glm::mat4();
	ProjectionMatrix = glm::mat4();

	direction = glm::vec3();
	right = glm::vec3();
	up = glm::vec3();
	position = glm::vec3();

	horizontalAngle = 3.14f;
	verticalAngle = 3.14f;
}

void Input::deleteVertex()
{
	pointsCloud.clear();
}

float Input::getDistance(float xa, float ya, Vertex v)
{
	float x = xa - v.x;
	float y = ya - v.y;

	return sqrtf(pow(x, 2) + pow(y, 2));
}

void Input::select(float x, float y)
{
	//TODO : select point
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) //Create point
	{
		if (xpos > 310)
		{
			Color col(1.0f, 1.0f, 1.0f);
			Vertex newPoint = Vertex(-1.0f + 2 * xpos / width, 1.0f - 2 * ypos / height, 0.0, col.x, col.y, col.z);
			pointsCloud.push_back(newPoint);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) // Move point
	{
		//TODO : move point
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) //Stop moving a point
	{
		if (movingPoint) 
		{
			movingPoint = false;
		}
	}
}

void Input::keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS || key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) // Finish shape
	{
		if (pointsCloud.size() > 2)
		{
			//TODO : Create shape
			pointsCloud.clear();
		}
	}

	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) // Enable point movement
	{
		wantToMovePoint = true;
	}

	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) // Disable point movement
	{
		wantToMovePoint = false;
	}

	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) // Clear scene
	{
		selectedPointId = NULL;
	}

	if (key == GLFW_KEY_V && action == GLFW_PRESS) // Switch to 3D view
	{
		enable3DViewport = !enable3DViewport;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) // Enable wireframe
	{
		enableWireframe = !enableWireframe;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS) //Display normals
	{
		enableNormal = !enableNormal;
	}
}

void Input::computeMatricesFromInputs(GLFWwindow* win, GLint winWidth, GLint winHeight)
{
	float speed = 5.0f;
	float mouseSpeed = 0.5f;

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	if(enable3DViewport)
		glfwSetCursorPos(win, winWidth / 2, winHeight / 2);

	horizontalAngle -= mouseSpeed * deltaTime * float(winWidth / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(winHeight / 2 - ypos);

	//Position
	//Forward
	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Backward
	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Right
	if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Left
	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwDestroyWindow(win);
		exit(0);
	}

	if (enable3DViewport)
	{
		direction = glm::vec3( 
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
	}
	else
	{
		position = glm::vec3();
		direction = glm::vec3(0, 0, -1);
		right = glm::vec3(1, 0, 0);
	}

	up = glm::cross(right, direction);

	//Matrix
	ProjectionMatrix = glm::perspective(glm::radians(60.f), float(winWidth) / float(winHeight), 0.01f, 100.0f);

	ViewMatrix = glm::lookAt(
		position,
		position + direction,
		up
	);
}