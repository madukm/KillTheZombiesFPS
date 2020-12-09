#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include "../../../shared/logger.hpp"

class Shader
{
	public:
		Shader();
		~Shader();

		void createFromFiles(std::string vertexFileName, std::string fragmentFileName);
		void useShader();
		GLuint getProgramId() const { return _programId; }

		//---------- Getters and Setters ----------//
		GLuint getModelLocation() { return _uniformModel; }
		GLuint getViewLocation() { return _uniformView; }
		GLuint getProjectionLocation() { return _uniformProjection; }

	private:
		std::string readFile(std::string fileName);
		void compileShader(std::string vertexCode, std::string fragmentCode);
		void addShader(std::string shaderCode, GLenum shaderType);

		GLuint _programId;

		// Uniforms
		GLuint _uniformModel;
		GLuint _uniformView;
		GLuint _uniformProjection;

		Logger* _log;
};

#endif// SHADER_H
