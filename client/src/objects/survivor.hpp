#ifndef SURVIVOR_H
#define SURVIVOR_H
#include <vector>
#include "object.hpp"
#include "../render/shader.hpp"
#include "../render/mesh.hpp"
#include "../render/texture.hpp"
#include "nlohmann/json.hpp"
using namespace nlohmann;

class Survivor : public Object
{
	public:
		Survivor(Shader* shader, 
				glm::vec3 position = {0,0,0}, 
				glm::vec3 rotation = {0,0,0},
				glm::vec3 scale = {1,1,1});
		~Survivor();

		void draw();
		void move(float dt);

		Shader* getShader() const { return _shader; }
		int getId() const { return _id; }
		float getHealth() const { return _health; }
		float getPower() const { return _power; }
		std::string getName() const { return _name; }
		bool getFly() const { return _fly; }
		glm::vec3 getFront() const { return _front; }
		int getMovingForward() const { return _movingForward; }
		int getMovingLeft() const { return _movingLeft; }


		void setId(int id) { _id = id; }
		void setHealth(float health) { _health = health; }
		void setPower(float power) { _power = power; }
		void setName(std::string name) { _name = name; }
		void setFly(bool fly) { _fly = fly; }
		void setFront(glm::vec3 front) { _front = front; }
		void setMovingForward(int movingForward) { _movingForward = movingForward; }
		void setMovingLeft(int movingLeft) { _movingLeft = movingLeft; }

		static Mesh* mesh;
		static Texture* texture;

	private:
		int _id;
		float _health;
		float _power;
		std::string _name;

		// Predict movement
		bool _fly;
		glm::vec3 _front;
		int _movingForward;
		int _movingLeft;
};

#endif// SURVIVOR_H
