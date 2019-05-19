#pragma once
#include <string>
class Rect
{
private:
	static int program;
	unsigned vao;
	unsigned tex;
	float x;
	float y;
	float z;
	float w;
	float h;
	static void initProgram();
	void init();
	Rect(
		float x, float y, float z, float w, float h, unsigned tex
	);
public:
	void show();
	static Rect * makeNew(
		float x, float y, float z, float w, float h, unsigned tex
	);
	~Rect();
};

