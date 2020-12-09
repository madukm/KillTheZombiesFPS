#include "survivor.hpp"
#include <glm/gtc/type_ptr.hpp>

Mesh* Survivor::mesh = nullptr;
Texture* Survivor::texture = nullptr;

Survivor::Survivor(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
	Object(shader, position, rotation, scale)
{

}

Survivor::~Survivor()
{

}

void Survivor::draw()
{
	//---------- Draw garage ----------//
	glm::mat4 mat = getModelMat();
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	texture->bind();
	mesh->draw();
}
