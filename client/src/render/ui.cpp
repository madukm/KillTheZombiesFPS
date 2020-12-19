#include "ui.hpp"
#include "window.hpp"
#include <glm/gtc/type_ptr.hpp>

UI::UI(Shader* shader):
	_shader(shader)
{
	loadAssets();
}

UI::~UI()
{
	for(auto& mesh : _meshes)
	{
		delete mesh;
		mesh = nullptr;
	}

	for(auto& texture : _textures)
	{
		delete texture;
		texture = nullptr;
	}
}

void UI::draw()
{
	//--------------------------//
	//-------- Draw 3D ---------//
	//--------------------------//
	
	// Zombies life bar
	_shader->useOnlyColor();
	_shader->setProcessLight(false);
	for(auto zombie : _zombies)
	{
		float life = zombie->getHealth();
		// TODO use camera position to rotate
		// TODO calculate life bar foreground
		//---------- Life bar background ----------//
		_shader->setColor(glm::vec3(0.2,0.2,0.2));
		glm::mat4 mat = zombie->getModelMat();
		mat = glm::translate(mat, glm::vec3(0,2.5,0));
		mat = glm::scale(mat, glm::vec3(2,.2,1));
		mat = glm::transpose(mat);
		glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));
		
		_meshes[0]->draw();

		//---------- Life bar foreground ----------//
		_shader->setColor(glm::vec3(0.2,1.0,0.2));
		mat = zombie->getModelMat();
		mat = glm::translate(mat, glm::vec3(-2*(1-life),2.5,.01));
		mat = glm::scale(mat, glm::vec3(2*life,.2,1));
		mat = glm::transpose(mat);
		glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));

		_meshes[0]->draw();
	}


	//--------------------------//
	//-------- Draw 2D ---------//
	//--------------------------//
	_shader->useOnlyTexture();
	// View matrix
	glUniformMatrix4fv(_shader->getViewLocation(), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	// Projection matrix
	glUniformMatrix4fv(_shader->getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	//---------- Sight ----------//
	// Model matrix
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(0, 0, -0.1f));
	mat = glm::scale(mat, glm::vec3(0.03f, 0.03f*Window::ratio, 1.00f));
	mat = glm::transpose(mat);
	glUniformMatrix4fv(_shader->getModelLocation(), 1, GL_TRUE, glm::value_ptr(mat));

	_textures[0]->bind();
	_meshes[0]->draw();

}

void UI::loadAssets()
{
	_meshes.push_back(new Mesh("square.obj"));

	_textures.push_back(new Texture("sight0.png"));
	_textures.push_back(new Texture("sight1.png"));
}
