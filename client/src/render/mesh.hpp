#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <GL/glew.h>
#include "vertex.hpp"
#include "../../../shared/logger.hpp"

class Mesh
{
	public:
		Mesh(std::string fileName);
		~Mesh();

		void loadFile(std::string fileName);
		void setupMesh();
		void draw();

	private:
		std::vector<std::string> splitLine(const std::string s, char delim);

		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		GLuint _VAO, _VBO, _EBO;

		Logger* _log;
};

#endif// MESH_H
