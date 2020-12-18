#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include "../render/shader.hpp"
#include "../../../shared/json.hpp"
using json = nlohmann::json; 

class Object
{
	public:
		Object(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		virtual void draw() = 0;
		//virtual json toJson() = 0;
		//virtual void fromJson() = 0;

		glm::mat4 getModelMat();

		glm::vec3 getPosition() { return _position; };
		glm::vec3 getRotation() { return _rotation; };
		glm::vec3 getScale() { return _scale; };
		void setPosition(glm::vec3 position) { _position=position; };
		void setRotation(glm::vec3 rotation) { _rotation=rotation; };
		void setScale(glm::vec3 scale) { _scale=scale; };

	protected:
		Shader* _shader;

		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;
};

#endif// OBJECT_H
