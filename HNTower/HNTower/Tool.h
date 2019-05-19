#pragma once
#include <string>
#include <vector>

class Tool
{
private:
	Tool();
public:
	static void checkStatus(int shader);
	static void checkProgramStatus(int program);
	static void getSingleModel(
		std::string src,
		std::vector<float> &vertices,
		std::vector<float> &normals,
		int &numVers);
	static void getSingleModel(
		std::string src,
		std::vector<float> &vertices,
		std::vector<float> &normals,
		std::vector<float> &coords,
		int &numVers);
	~Tool();
};

