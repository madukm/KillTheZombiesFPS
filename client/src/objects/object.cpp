#include "object.hpp"
#include <glm/gtc/matrix_transform.hpp>

Object::Object(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
	_shader(shader), _position(position), _rotation(rotation), _scale(scale)
{

}

glm::mat4 Object::getModelMat()
{
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(mat, _position);
	mat = glm::rotate(mat, glm::radians(_rotation.x) ,glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.y) ,glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, glm::radians(_rotation.z) ,glm::vec3(0, 0, 1));
	mat = glm::scale(mat, _scale);
	return mat;
}
