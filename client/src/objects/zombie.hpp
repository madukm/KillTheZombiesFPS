#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <vector>
#include "object.hpp"
#include "../render/shader.hpp"
#include "../render/mesh.hpp"
#include "../render/texture.hpp"
#include "nlohmann/json.hpp"
using namespace nlohmann;

class Zombie : public Object
{
	public:
		Zombie(Shader* shader, 
				glm::vec3 position = {0,0,0}, 
				glm::vec3 rotation = {0,0,0},
				glm::vec3 scale = {1,1,1});
		~Zombie();

		void draw();
		void move(float dt);

		Shader* getShader() const { return _shader; }
		int getId() const { return _id; }
		float getHealth() const { return _health; }
		glm::vec3 getFront() const { return _front; }
		int getMovingForward() const { return _movingForward; }
		int getMovingLeft() const { return _movingLeft; }

		void setId(int id) { _id = id; }
		void setHealth(float health) { _health = health; }
		void setFront(glm::vec3 front) { _front = front; }
		void setMovingForward(int movingForward) { _movingForward = movingForward; }
		void setMovingLeft(int movingLeft) { _movingLeft = movingLeft; }


		static Mesh* mesh;
		static Texture* texture;

	private:
		int _id;
		float _health;
		glm::vec3 _front;
		int _movingForward;
		int _movingLeft;
};

#endif// ZOMBIE_H
