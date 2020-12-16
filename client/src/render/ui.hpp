#ifndef UI_H
#define UI_H
#include <vector>
#include "../render/shader.hpp"
#include "../render/mesh.hpp"
#include "../render/texture.hpp"

class UI
{
	public:
		UI(Shader* shader);
		~UI();

		void draw();

	private:
		void loadAssets();

		Shader* _shader;
		std::vector<Mesh*> _meshes;
		std::vector<Texture*> _textures;
};

#endif// UI_H
