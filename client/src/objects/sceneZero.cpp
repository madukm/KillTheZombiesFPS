#include "sceneZero.hpp"

SceneZero::SceneZero(Shader* shader)
{
	// Create scene blocks
	// Ground
	_blocks.push_back(new Block(shader, glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(100,0.1,100), glm::vec3(0.8,0.5,0.3)));

	// Limit walls
	_blocks.push_back(new Block(shader, glm::vec3(0,2,50), glm::vec3(0,0,0), glm::vec3(100,4,0.3), glm::vec3(0.8,0.1,0.1)));
	_blocks.push_back(new Block(shader, glm::vec3(0,2,-50), glm::vec3(0,0,0), glm::vec3(100,4,0.3), glm::vec3(0.8,0.1,0.1)));
	_blocks.push_back(new Block(shader, glm::vec3(50,2,0), glm::vec3(0,90,0), glm::vec3(100,4,0.3), glm::vec3(0.8,0.1,0.1)));
	_blocks.push_back(new Block(shader, glm::vec3(-50,2,0), glm::vec3(0,90,0), glm::vec3(100,4,0.3), glm::vec3(0.8,0.1,0.1)));

	//---------- Obstacles ----------//
	// Corner Obstacles
	_blocks.push_back(new Block(shader, glm::vec3(-30,2,40), glm::vec3(0,0,0), glm::vec3(40,4,0.8), glm::vec3(0.1,0.1,0.8)));
	_blocks.push_back(new Block(shader, glm::vec3(30,2,40), glm::vec3(0,0,0), glm::vec3(40,4,0.8), glm::vec3(0.1,0.1,0.8)));

	_blocks.push_back(new Block(shader, glm::vec3(-30,2,-40), glm::vec3(0,0,0), glm::vec3(40,4,0.8), glm::vec3(0.1,0.8,0.2)));
	_blocks.push_back(new Block(shader, glm::vec3(30,2,-40), glm::vec3(0,0,0), glm::vec3(40,4,0.8), glm::vec3(0.1,0.8,0.2)));

	// Middle line
	_blocks.push_back(new Block(shader, glm::vec3(0,2,0), glm::vec3(0,0,0), glm::vec3(80,4,0.8), glm::vec3(0.1,0.6,0.8)));

	// Vertical obstacles
	_blocks.push_back(new Block(shader, glm::vec3(-20,2,25), glm::vec3(0,90,0), glm::vec3(30,4,0.8), glm::vec3(0.1,0.1,0.8)));
	_blocks.push_back(new Block(shader, glm::vec3(20,2,25), glm::vec3(0,90,0), glm::vec3(30,4,0.8), glm::vec3(0.1,0.1,0.8)));
	_blocks.push_back(new Block(shader, glm::vec3(-20,2,-25), glm::vec3(0,90,0), glm::vec3(30,4,0.8), glm::vec3(0.1,0.8,0.1)));
	_blocks.push_back(new Block(shader, glm::vec3(20,2,-25), glm::vec3(0,90,0), glm::vec3(30,4,0.8), glm::vec3(0.1,0.8,0.1)));
}

SceneZero::~SceneZero()
{
	for(auto& block : _blocks)
	{
		if(block != nullptr)
		{
			delete block;
			block = nullptr;
		}
	}
}

void SceneZero::draw()
{
	for(auto& block : _blocks)
	{
		block->draw();
	}
}
