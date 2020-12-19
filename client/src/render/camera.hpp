#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <vector>
#include "../../../shared/logger.hpp"

class Block;

class Camera
{
	public:
		Camera(float windowRatio);
		~Camera();

		void updateOnKey(int key, int scancode, int action, int mods);
		void updateOnMouse(double xpos, double ypos);
		void update(double dt);
		void printInfo();

		const float* getView();
		const float* getProjection();

		//---------- Getters and Setters ----------//
		glm::vec3 getPosition() const { return _position; }
		glm::vec3 getRotation() const { return glm::vec3(0,_yaw-90,0); }
		void setPosition(glm::vec3 pos) { _position=pos; }
		void setFront(glm::vec3 front) { _front=front; }
		void setSceneBlocks(std::vector<Block*> blocks) { _sceneBlocks = blocks; }

	private:
		// Check if the position is colliding with any block
		bool detectCollision(glm::vec3 position);

		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _front;
		float _speed;
		float _ratio;

		// Mouse handling
		bool _firstMouse;
		float _yaw, _pitch;
		float _lastX, _lastY;

		// Matrices
		glm::mat4 _view;
		glm::mat4 _projection;

		// Handling freeCamera
		int _movingForward;
		int _movingLeft;

		// game parameters
		bool _fly;
		bool _collisionDetection;
		std::vector<Block*> _sceneBlocks;

		Logger* _log;
};

#endif// CAMERA_H
