#include "ObjLoader2.h"
#include <stdio.h>
#include <string>
#include <cstring>
//#include "ObjLoader.h"

using namespace MATH;
/// Since theses vectors are static I need to initialize them here
/// it's the tricky bit about static 
std::vector<Vec3> ObjLoader2::normals;
std::vector<Vec3> ObjLoader2::vertices;
std::vector<Vec2> ObjLoader2::uvCoords;

bool ObjLoader2::loadOBJ2(const char* path) {
	printf("Loading OBJ file %s...\n", path);
	normals.clear();
	vertices.clear();
	uvCoords.clear();
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vec3> temp_vertices;
	std::vector<Vec2> temp_uvs;
	std::vector<Vec3> temp_normals;


	FILE* file;
	fopen_s(&file, path, "r");
	if (file == nullptr) {
		printf("Can't open your file %s \n", path);
		return false;
	}

	while (true) {
		char lineHeader[128];
		/// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF) {
			break; /// End Of File
		}
		if (strcmp(lineHeader, "v") == 0) {
			Vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; /// Grr 
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("Error: Can't read the file - this is a dumb reader");
				return false;
			}
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
		else {
			/// the data is probably a comment, eat up the rest of the line and throw it away
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	/// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		Vec3 vertex = temp_vertices[vertexIndex - 1];
		Vec2 uv = temp_uvs[uvIndex - 1];
		Vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvCoords.push_back(uv);
		normals.push_back(normal);

	}
	return true;
}
