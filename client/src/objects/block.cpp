#include "block.hpp"
#include <glm/gtc/type_ptr.hpp>

Mesh* Block::mesh = nullptr;
//Texture* Block::texture = nullptr;

Block::Block(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color):
	Object(shader, position, rotation, scale), _color(color)
{

}

Block::~Block()
{

}

void Block::draw()
{
	_shader->useOnlyColor();
	_shader->setColor(_color);

	glm::mat4 mat = getModelMat();
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	//texture->bind();
	mesh->draw();
}
