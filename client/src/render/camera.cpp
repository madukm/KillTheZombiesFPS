//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.hpp"

Camera::Camera(float windowRatio):
	_ratio(windowRatio), _firstMouse(true)
{
	_position = {0,1.5,0};
	_up = {0,1,0};
	_front = {0,0,-1};

	_speed = 60;

	_yaw = -90;
	_pitch = 0;

	_movingForward = 0;
	_movingLeft = 0;

	_log = new Logger("Camera");
}

Camera::~Camera()
{
	if(_log != nullptr)
	{
		delete _log;
		_log = nullptr;
	}
}

void Camera::updateOnKey(int key, int scancode, int action, int mods)
{
	switch(key)
	{
		case GLFW_KEY_W:
			if(action == GLFW_RELEASE)
				_movingForward = 0;
			else
				_movingForward = 1;
			break;
		case GLFW_KEY_A:
			if(action == GLFW_RELEASE)
				_movingLeft = 0;
			else
				_movingLeft = -1;
			break;
		case GLFW_KEY_S:
			if(action == GLFW_RELEASE)
				_movingForward = 0;
			else
				_movingForward = -1;
			break;
		case GLFW_KEY_D:
			if(action == GLFW_RELEASE)
				_movingLeft = 0;
			else
				_movingLeft = 1;
			break;
	}
}

void Camera::updateOnMouse(double xpos, double ypos)
{
	if(_firstMouse)
	{
		_firstMouse = false;
		_lastX = xpos;
		_lastY = ypos;
	}

	float xoffset = xpos-_lastX;
	float yoffset = ypos-_lastY;

	float sensitivity = 600;
	xoffset*=sensitivity;
	yoffset*=sensitivity;

	_yaw+=xoffset;
	_pitch-=yoffset;

	if(_pitch>=89)_pitch=89;
	if(_pitch<=-89)_pitch=-89;

	if(_yaw>=360)_yaw-=360;
	if(_yaw<=0)_yaw+=360;

	glm::vec3 front;
	front.x = cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
    _front = glm::normalize(front);

	_lastX = xpos;
	_lastY = ypos;
}

void Camera::update(double dt)
{
	glm::vec3 planeFront = _front;
	planeFront.y = 0;
	planeFront = glm::normalize(planeFront);

	_position += planeFront*_speed * float(_movingForward*dt);
	_position += glm::normalize(glm::cross(_front, _up))*_speed * float(_movingLeft*dt);

	_view = glm::lookAt(_position, _position+_front, _up);
	_projection = glm::perspective(
			glm::radians(60.0f),
			_ratio,
			0.1f,
			1000.0f
		);
}

void Camera::printInfo()
{
	_log->log("Position: ("
			+std::to_string(_position.x)+", "
			+std::to_string(_position.y)+", "
			+std::to_string(_position.z)+") "+
			"Up: ("
			+std::to_string(_up.x)+", "
			+std::to_string(_up.y)+", "
			+std::to_string(_up.z)+") "+
			"Left: ("
			+std::to_string(_front.x)+", "
			+std::to_string(_front.y)+", "
			+std::to_string(_front.z)+") "
			, DEBUG);

	_log->log("Pitch: "
			+std::to_string(_pitch)+", "
			"Yaw: "
			+std::to_string(_yaw)
			, DEBUG);
}

const float* Camera::getView()
{
	return glm::value_ptr(_view);
}

const float* Camera::getProjection()
{
	return glm::value_ptr(_projection);
}
