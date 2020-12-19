//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-18                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "../objects/block.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.hpp"

Camera::Camera(float windowRatio):
	_ratio(windowRatio), _firstMouse(true), _fly(false), _collisionDetection(true), _player(nullptr)
{
	_position = {0,1.8,10};
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
		case GLFW_KEY_F:
			if(action == GLFW_PRESS)
				_fly = !_fly;
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
	if(!_fly)
	{
		planeFront.y = 0;
		planeFront = glm::normalize(planeFront);
	}

	glm::vec3 nextPos = _position;
	nextPos += planeFront*_speed * float(_movingForward*dt);
	nextPos += glm::normalize(glm::cross(_front, _up))*_speed * float(_movingLeft*dt);

	// Move only if not colliding
	if(detectCollision(nextPos) == false)
		_position = nextPos;

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

//------------------------------//
//----- Collision Detection ----//
//------------------------------//

struct CollisionBox
{
	glm::vec3 size;
	glm::vec3 axis0;
	glm::vec3 axis1;
	glm::vec3 axis2;
};

bool overlapOnAxis(CollisionBox one, CollisionBox two, glm::vec3 axis, glm::vec3 toCenter);
float transformToAxis(CollisionBox box, glm::vec3 axis);

bool Camera::detectCollision(glm::vec3 position)
{
	if(_player==nullptr)
		return false;

	for(auto block : _sceneBlocks)
	{
		glm::vec3 position = block->getPosition();
		glm::vec3 rotation = block->getRotation();
		glm::vec3 scale = block->getScale();

		// Block collision box info
		glm::mat4 model = block->getModelMat();
		CollisionBox blockBox = {
			block->getScale()/2.0f,
			glm::vec3(model[0][0], model[1][0], model[2][0]),
			glm::vec3(model[0][1], model[1][1], model[2][1]),
			glm::vec3(model[0][2], model[1][2], model[2][2])
		};

		// Player collision box info
		glm::mat4 modelCamera = _player->getModelMat();
		CollisionBox playerBox = {
			glm::vec3(1,1,1)/2.0f,
			glm::vec3(modelCamera[0][0], modelCamera[1][0], modelCamera[2][0]),
			glm::vec3(modelCamera[0][1], modelCamera[1][1], modelCamera[2][1]),
			glm::vec3(modelCamera[0][2], modelCamera[1][2], modelCamera[2][2])
		};
		std::cout << "---------------------- BLOCK ---------------------- " << std::endl;
		std::cout << glm::to_string(blockBox.size) << std::endl;
		std::cout << glm::to_string(position) << std::endl;

		// Vector between the two centres
		glm::vec3 toCenter = position - _position;

		// Calculating collision between boxes using separating axis theorem

		// Check block box axes
		bool boxAxes = overlapOnAxis(blockBox, playerBox, blockBox.axis0, toCenter) &&
				overlapOnAxis(blockBox, playerBox, blockBox.axis1, toCenter) &&
				overlapOnAxis(blockBox, playerBox, blockBox.axis2, toCenter);

		// Check player box axes
		bool playerAxes = overlapOnAxis(blockBox, playerBox, playerBox.axis0, toCenter) &&
				overlapOnAxis(blockBox, playerBox, playerBox.axis1, toCenter) &&
				overlapOnAxis(blockBox, playerBox, playerBox.axis2, toCenter);
		
		// Check cross product axes
		bool crossAxes = overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis0, playerBox.axis0), toCenter) &&
				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis0, playerBox.axis1), toCenter) &&
				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis0, playerBox.axis2), toCenter) &&

				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis1, playerBox.axis0), toCenter) &&
				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis1, playerBox.axis1), toCenter) &&
				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis1, playerBox.axis2), toCenter) &&
				
				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis2, playerBox.axis0), toCenter) &&
				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis2, playerBox.axis1), toCenter) &&
				overlapOnAxis(blockBox, playerBox, glm::cross(blockBox.axis2, playerBox.axis2), toCenter);

		
		// If overlap in all axes, the boxes are colliding
		if(boxAxes && playerAxes && crossAxes)
			return true;
	}

	return false;
}

float transformToAxis(CollisionBox box, glm::vec3 axis)
{
    return
        box.size.x * abs(glm::dot(axis, box.axis0)) +
        box.size.y * abs(glm::dot(axis, box.axis1)) +
        box.size.z * abs(glm::dot(axis, box.axis2));
}

bool overlapOnAxis(CollisionBox one, CollisionBox two, glm::vec3 axis, glm::vec3 toCenter)
{
    // Project the half-size of one onto axis
    float oneProject = transformToAxis(one, axis);
    float twoProject = transformToAxis(two, axis);

    // Project this onto the axis
    float distance = abs(glm::dot(toCenter, axis));

	std::cout << "----------" << std::endl;
	//std::cout << "One" << std::endl;
	//std::cout << "0: " << glm::to_string(one.axis0) << std::endl;
	//std::cout << "1: " << glm::to_string(one.axis1) << std::endl;
	//std::cout << "2: " << glm::to_string(one.axis2) << std::endl;
	//std::cout << "Two" << std::endl;
	//std::cout << "0: " << glm::to_string(two.axis0) << std::endl;
	//std::cout << "1: " << glm::to_string(two.axis1) << std::endl;
	//std::cout << "2: " << glm::to_string(two.axis2) << std::endl;
	//std::cout << "Axis " << glm::to_string(axis) << std::endl;

	std::cout << "One Proj " << oneProject << std::endl;
	std::cout << "Two Proj " << twoProject << std::endl;
	std::cout << "ToCenter " << glm::to_string(toCenter) << std::endl;
	std::cout << "Distance " << distance << std::endl;
	//std::cout << distance << " <= " << oneProject << " + " << twoProject << std::endl;
	if((distance <= oneProject + twoProject) == false)
	{
		std::cout << "Not colliding" << std::endl;
	}

    // Check for overlap
    return (distance <= oneProject + twoProject);
}
