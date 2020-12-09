#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <GL/glew.h>
#include "../../../shared/logger.hpp"

class Texture
{
	public:
		Texture(std::string fileName);
		~Texture();

		void bind();

	private:
		void loadTexture(std::string fileName);

		int _width;
		int _height;
		int _channels;
		GLuint _texId;

		Logger* _log;
};

#endif// TEXTURE_H
