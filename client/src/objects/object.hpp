#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include "../render/shader.hpp"

class Object
{
	public:
		Object(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		virtual void draw() = 0;

		glm::mat4 getModelMat();

	protected:
		Shader* _shader;

		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;
};

#endif// OBJECT_H
