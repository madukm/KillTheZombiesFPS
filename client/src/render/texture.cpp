#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string fileName)
{
	_log = new Logger("Texture");

	loadTexture(fileName);
}

Texture::~Texture()
{
	if(_log != nullptr)
	{
		delete _log;
		_log = nullptr;
	}
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0);// Activate first texture unit
	glBindTexture(GL_TEXTURE_2D, _texId);
}

void Texture::loadTexture(std::string fileName)
{
	// Bind texture
	glGenTextures(1, &_texId);
	glBindTexture(GL_TEXTURE_2D, _texId); 

	// Load image
	unsigned char *data = stbi_load(("assets/textures/"+fileName).c_str(), &_width, &_height, &_channels, 0); 
	if(data)
	{
		_log->log(fileName + " loaded successfully: " 
				+ std::to_string(_width) + "px X "
				+ std::to_string(_height) + "px X "
				+ std::to_string(_channels) + ".", DEBUG);

		// Set the texture wrapping/filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Generate texture
		if(_channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if(_channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
		{
			_log->log("Strange number of channels!", DEBUG);
			exit(1);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		_log->log("Failed to load " + fileName, DEBUG);

	}

	stbi_image_free(data);
}
