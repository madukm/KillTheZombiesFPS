#include <fstream>
#include "shader.hpp"

Shader::Shader()
{
	_log = new Logger("Shader");
}
Shader::~Shader()
{
	if(_log != nullptr)
	{
		delete _log;
		_log = nullptr;
	}
}

void Shader::setColor(glm::vec3 color)
{
	glUniform3f(_uniformColor, color.x, color.y, color.z);
}

void Shader::useOnlyColor()
{
	glUniform1i(_uniformOnlyColor, 1);
}

void Shader::useOnlyTexture()
{
	glUniform1i(_uniformOnlyColor, 0);
}

void Shader::setProcessLight(bool process)
{
	glUniform1i(_uniformProcessLight, (int)process);
}

void Shader::createFromFiles(std::string vertexFileName, std::string fragmentFileName)
{
	std::string vertexCode = readFile(vertexFileName);
	std::string fragmentCode = readFile(fragmentFileName);

	compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(std::string fileName)
{
	std::string content;
	std::ifstream fileStream(("src/shaders/"+fileName).c_str(), std::ios::in);

	if(!fileStream.is_open())
	{
		_log->log("Failed to read"+ fileName + "! File doesn't exist.", DEBUG);
		return "";
	}

	std::string line = "";
	while(!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line+"\n");
	}

	fileStream.close();
	return content;
}

void Shader::compileShader(std::string vertexCode, std::string fragmentCode)
{
	// Create program
	_programId = glCreateProgram();

	if(!_programId)
	{
		_log->log("Error creating shader program!", WARN);
		exit(1);
	}

	// Add shaders
	addShader(vertexCode, GL_VERTEX_SHADER);
	addShader(fragmentCode, GL_FRAGMENT_SHADER); 

	GLint result = 0;
	GLchar eLog[1024] = {0};

	// Link program
	glLinkProgram(_programId);
	glGetProgramiv(_programId, GL_LINK_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(_programId, sizeof(eLog), NULL, eLog);
		_log->log("Error linking program: '" + std::string(eLog)+ "'", WARN);
		exit(1);
	}

	// Validade program
	glValidateProgram(_programId);
	glGetProgramiv(_programId, GL_VALIDATE_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(_programId, sizeof(eLog), NULL, eLog);
		_log->log("Error validating program: '" + std::string(eLog)+ "'", WARN);
		exit(1);
	}

	//---------- Get uniforms ----------//
	_uniformModel = glGetUniformLocation(_programId, "model");
	_uniformView = glGetUniformLocation(_programId, "view");
	_uniformProjection = glGetUniformLocation(_programId, "projection");
	_uniformColor = glGetUniformLocation(_programId, "color");
	_uniformOnlyColor = glGetUniformLocation(_programId, "onlyColor");
	_uniformProcessLight = glGetUniformLocation(_programId, "processLight");
}

void Shader::useShader()
{
	glUseProgram(_programId);
}

void Shader::addShader(std::string shaderCode, GLenum shaderType)
{
	// Create shader
	GLuint shader = glCreateShader(shaderType);

	// Compile shader
	const GLchar* code[1];
	code[0] = shaderCode.c_str();

	GLint codeLength[1];
	codeLength[0] = shaderCode.size();

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		//Log::error("Shader", "Error compiling the " + std::to_string(shaderType)+ " shader: '" + std::string(eLog)+ "'");
		exit(1);
	}

	// Attach shader to program
	glAttachShader(_programId, shader);
}
