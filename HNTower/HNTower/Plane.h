#pragma once
#include <string>

class Plane
{
private:
	static int program;
	unsigned vao;
	unsigned tex;
	int numVers;
	float x;
	float y;
	float z;
	static void initProgram();
	void init(std::string &src);
	Plane(std::string &src, float x, float y, float z);
public:
	void show(bool isDrawToFB);
	void drawToFramebuffer();
	static Plane * makeNew(std::string src, float x, float y, float z);
	~Plane();
};