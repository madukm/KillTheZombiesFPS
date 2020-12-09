#ifndef BLOCK_H
#define BLOCK_H
#include "object.hpp"
#include "../render/shader.hpp"
#include "../render/mesh.hpp"
#include "../render/texture.hpp"

class Block : public Object
{
	public:
		Block(Shader* shader, 
				glm::vec3 position = {0,0,0}, 
				glm::vec3 rotation = {0,0,0},
				glm::vec3 scale = {1,1,1});
		~Block();

		void draw();

		static Mesh* mesh;
		static Texture* texture;
};

#endif// BLOCK_H
