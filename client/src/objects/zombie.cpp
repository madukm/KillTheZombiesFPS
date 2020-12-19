#include "zombie.hpp"
#include <glm/gtc/type_ptr.hpp>

Mesh* Zombie::mesh = nullptr;
Texture* Zombie::texture = nullptr;

Zombie::Zombie(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
	Object(shader, position, rotation, scale), _health(1.0f)
{

}

Zombie::~Zombie()
{

}

void Zombie::draw()
{
	_shader->useOnlyTexture();

	glm::mat4 mat = getModelMat();
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	texture->bind();
	mesh->draw();

}
