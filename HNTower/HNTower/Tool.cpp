#include "Tool.h"
#include <GL\glew.h>
#include <stdexcept>
#include <assimp\scene.h>
#include <assimp\mesh.h>
#include <assimp\vector3.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

Tool::Tool()
{
}


Tool::~Tool()
{
}

void Tool::getSingleModel(
	std::string src,
	std::vector<float> &vertices,
	std::vector<float> &normals,
	int &numVers
) {
	Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(src, aiProcess_Triangulate);
	if (!scene) {
		throw std::runtime_error("Null pointer error!");
	}
	aiMesh **mesh = scene->mMeshes;
	aiVector3D *ver = mesh[0]->mVertices;
	aiVector3D *norm = mesh[0]->mNormals;
	numVers = mesh[0]->mNumVertices;
	for (int i = 0; i < numVers; i++) {
		vertices.push_back(ver[i].x);
		vertices.push_back(ver[i].y);
		vertices.push_back(ver[i].z);
	}
	if (norm) {
		for (int i = 0; i < numVers; i++) {
			normals.push_back(norm[i].x);
			normals.push_back(norm[i].y);
			normals.push_back(norm[i].z);
		}
	}
}

void Tool::getSingleModel(
	std::string src,
	std::vector<float> &vertices,
	std::vector<float> &normals,
	std::vector<float> &coords,
	int &numVers
) {
	Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(src, aiProcess_Triangulate);
	if (!scene) {
		throw std::runtime_error("Null pointer error in Tool!");
	}
	aiMesh **mesh = scene->mMeshes;
	aiVector3D *ver = mesh[0]->mVertices;
	aiVector3D *norm = mesh[0]->mNormals;
	aiVector3D *coord = mesh[0]->mTextureCoords[0];
	numVers = mesh[0]->mNumVertices;
	for (int i = 0; i < numVers; i++) {
		vertices.push_back(ver[i].x);
		vertices.push_back(ver[i].y);
		vertices.push_back(ver[i].z);
	}
	if (norm) {
		for (int i = 0; i < numVers; i++) {
			normals.push_back(norm[i].x);
			normals.push_back(norm[i].y);
			normals.push_back(norm[i].z);
		}
	}
	if (coord) {
		for (int i = 0; i < numVers; i++) {
			coords.push_back(coord[i].x);
			coords.push_back(coord[i].y);
		}
	}
}

void Tool::checkStatus(int shader) {
	int compileSt;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSt);
	if (!compileSt) {
		char *infolog;
		std::string message;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compileSt);
		infolog = new char[compileSt];
		glGetShaderInfoLog(shader, compileSt, nullptr, infolog);
		message = infolog;
		delete[] infolog;
		throw std::runtime_error(message);
	}
}

void Tool::checkProgramStatus(int program) {
	int linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		char *infolog;
		std::string message;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &linkStatus);
		infolog = new char[linkStatus];
		glGetProgramInfoLog(program, linkStatus, nullptr, infolog);
		message = infolog;
		delete[] infolog;
		throw std::runtime_error(message);
	}
}
