#include "survivor.hpp"
#include <glm/gtc/type_ptr.hpp>

Mesh* Survivor::mesh = nullptr;
Texture* Survivor::texture = nullptr;

Survivor::Survivor(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
	Object(shader, position, rotation, scale), _health(1.0f)
{

}

Survivor::~Survivor()
{

}

void Survivor::draw()
{
	_shader->useOnlyTexture();

	glm::mat4 mat = getModelMat();
	mat = glm::transpose(mat);
    glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
	
	// Bind texture/VAO and draw
	texture->bind();
	mesh->draw();
}

json Survivor::getJson()
{
	json result = {};
	result["pos"] = {_position.x, _position.y, _position.z};
	result["rot"] = {_rotation.x, _rotation.y, _rotation.z};
	return result;
}

void Survivor::setJson(json state)
{
	if(state == nullptr)
		return;

	_position.x = state["pos"][0];
	_position.x = state["pos"][0];
	_position.x = state["pos"][0];

	_rotation.x = state["rot"][0];
	_rotation.x = state["rot"][0];
	_rotation.x = state["rot"][0];
}
