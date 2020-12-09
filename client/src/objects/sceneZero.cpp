#include "sceneZero.hpp"

SceneZero::SceneZero(Shader* shader)
{
	// Create scene blocks
	_blocks.push_back(new Block(shader, glm::vec3(0,-1,0), glm::vec3(0,0,0), glm::vec3(50,0.1,50)));
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
