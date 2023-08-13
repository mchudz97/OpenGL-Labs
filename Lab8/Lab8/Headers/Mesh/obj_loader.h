// References
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
// contact@opengl-tutorial.org

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include "vertex.h"
#include <glm.hpp>


// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide :
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc


bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);

void serialize(const char* path, std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	std::cout << "\nCreating binary file at " << path;
	std::ofstream out(path, std::ios::binary);
	unsigned long size1 = vertices.size();
	unsigned long size2 = indices.size();
	out.write(reinterpret_cast<char*>(&size1), sizeof(size1));
	out.write(reinterpret_cast<char*>(&size2), sizeof(size2));
	out.write(reinterpret_cast<char*>(&vertices[0]), size1 * sizeof(Vertex));
	out.write(reinterpret_cast<char*>(&indices[0]), size2 * sizeof(GLuint));
	out.close();
}

bool deserialize(const char* path, std::vector<Vertex> &vertices, std::vector<GLuint> &indices)
{
	std::ifstream in(path, std::ios::binary);
	if (!in.is_open())
		return false;
	std::cout << "\nBinary found.  Reading data from " << path;
	unsigned long size1;
	unsigned long size2;
	in.read(reinterpret_cast<char*>(&size1), sizeof(size1));
	in.read(reinterpret_cast<char*>(&size2), sizeof(size2));
	vertices.resize(size1);
	indices.resize(size2);
	in.read(reinterpret_cast<char*>(&vertices[0]), size1 * sizeof(Vertex));
	in.read(reinterpret_cast<char*>(&indices[0]), size2 * sizeof(GLuint));
	in.close();
	return true;
}



bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
)
{
	printf("Loading OBJ file %s ... ", path);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("can't open the file.\n");
		return false;
	}


	while (1)
	{

		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// read vertex coordinates
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);

		}
		// read texture coordinates
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			// Invert V coordinate since we will only use DDS texture,
			// which are inverted. Remove if you want to use TGA or BMP loaders.
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}
		// read normal vectors
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		// read faces (triangles)
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9)
			{
				printf("\nFile can't be read by our simple parser. ");
				printf("Try exporting with other options and make sure you export normals and uvs.\n");
				fclose(file);
				return false;
			}

			// Create new triangle
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	fclose(file);
	printf(" done.\n");
	return true;
}

bool loadOBJandSetIndices(const char* path,
	std::vector<Vertex>& outVertices,
	std::vector<GLuint>& outIndices
)
{

	std::string objName = std::string(path);
	int filenameIndex = objName.find("/");
	while (filenameIndex != -1) 
	{
		objName = objName.substr(filenameIndex + 1, objName.size() - 1);
		filenameIndex = objName.find("/");

	}
	filenameIndex = objName.find(".");
	objName = objName.substr(0, filenameIndex);

	if (deserialize(("binaries/" + objName + ".bin").c_str(), outVertices, outIndices))
		return true;

	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	
	GLuint actualIdx = 0;

	if (loadOBJ(path, temp_vertices, temp_uvs, temp_normals))
	{
		std::cout << "Setting up indices.. ";
		
		
		for (int i = 0; i < temp_vertices.size(); i++) {
			bool isIncluded = false;
			for (int j = 0; j < outVertices.size(); j++) {
				if (temp_vertices[i] == outVertices[j].v_position &&
					temp_uvs[i] == outVertices[j].v_tex &&
					temp_normals[i] == outVertices[j].v_normal)
				{
					outIndices.push_back(j);
					isIncluded = true;
					break;
				}
			}
			if (!isIncluded) {
				Vertex v;
				v.v_position = temp_vertices[i];
				v.v_tex = temp_uvs[i];
				v.v_normal = temp_normals[i];
				outVertices.push_back(v);
				outIndices.push_back(actualIdx++);
			}
		}
		std::cout << "Done" << "\n";
		serialize(("binaries/" + objName + ".bin").c_str(), outVertices, outIndices);
		return true;
		

	}
	std::cout << "Failed" << "\n";
	return false;
}
#endif