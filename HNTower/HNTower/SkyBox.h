#pragma once
#include <string>
class SkyBox
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
	void init();
	SkyBox(float x, float y, float z);
public:
	void show();
	static SkyBox * makeNew(float x, float y, float z);
	~SkyBox();
};

