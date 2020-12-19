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

		static Mesh* mesh;
		static Texture* texture;

		float getHealth() { return _health; }
		void setHealth(float health) { _health=health; }

	private:
		float _health;
};

#endif// ZOMBIE_H
