#ifndef SCENE_ZERO_H
#define SCENE_ZERO_H
#include <vector>
#include "block.hpp"

class SceneZero
{
	public:
		SceneZero(Shader* shader);
		~SceneZero();

		void draw();
	private:
		std::vector<Block*> _blocks;
};

#endif// SCENE_ZERO_H
