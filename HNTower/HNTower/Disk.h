#pragma once
#include <string>
class Disk
{
private:
	static int program;
	static const float STEP;
	unsigned vao;
	int numVers;
	float x;
	float y;
	float z;
	int tgIndex;
	float target[3][2];
	static void initProgram();
	void init(std::string &src);
	Disk(std::string &src, float x, float y, float z);
public:
	void show(bool isDrawToFB);
	void setTarget(
		float x1, float y1, float x2, float y2,
		float x3, float y3);
	void move();
	static Disk * makeNew(std::string src, float x, float y, float z);
	~Disk();
};

