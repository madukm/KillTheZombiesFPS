#ifndef UI_H
#define UI_H
#include <vector>
#include "../render/shader.hpp"
#include "../render/mesh.hpp"
#include "../render/texture.hpp"
#include "../objects/zombie.hpp"
#include "../objects/survivor.hpp"

class UI
{
	public:
		UI(Shader* shader);
		~UI();

		void draw();

		 void setZombies(std::vector<Zombie*> zombies) { _zombies = zombies; }
		 void setPlayers(std::vector<Survivor*> players) { _players = players; }

	private:
		void loadAssets();

		Shader* _shader;
		std::vector<Mesh*> _meshes;
		std::vector<Texture*> _textures;

		std::vector<Zombie*> _zombies;
		std::vector<Survivor*> _players;
};

#endif// UI_H
