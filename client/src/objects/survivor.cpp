#include "survivor.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

Mesh* Survivor::mesh = nullptr;
Texture* Survivor::texture = nullptr;

Survivor::Survivor(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
	Object(shader, position, rotation, scale), _id(-1), _health(1.0f), _power(1.0f), _name("survivor"),
	_fly(false), _front({0,0,0}), _movingForward(0), _movingLeft(0)
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

void Survivor::move(float dt)
{
	glm::vec3 planeFront = _front;
	if(!_fly)
	{
		planeFront.y = 0;
		planeFront = glm::normalize(planeFront);
	}

	float speed = 50.0f;
	_position += planeFront* (speed*_movingForward) * dt;
	_position += glm::normalize(glm::cross(_front, glm::vec3(0,1,0)))* (speed*_movingLeft) * dt;

	//std::cout << "CURR POSS " << glm::to_string(_position) << std::endl;
}
