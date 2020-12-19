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

		json getJson();
		void setJson(json state);
		Shader* getShader() const { return _shader; }

		static Mesh* mesh;
		static Texture* texture;

		float getHealth() { return _health; }

	private:
		float _health;
};

#endif// SURVIVOR_H
