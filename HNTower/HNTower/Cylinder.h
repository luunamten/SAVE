#pragma once
#include <string>
#include <stack>
#include "Disk.h"

class Cylinder
{
private:
	static int program;
	static const float HEIGHT;
	static void initProgram();
	unsigned vao;
	int numVers;
	float x;
	float y;
	float z;
	std::stack<Disk *> disks;
	void init(std::string &src);
	Cylinder(std::string &src, float x, float y, float z);
public:
	static Cylinder * makeNew(std::string src, float x, float y, float z);
	void show(bool drawToFramebuffer);
	void addDisk(Disk *disk);
	void pop();
	void push(Disk *disk);
	float getX();
	float getY();
	float getTopPos();
	size_t size();
	Disk * top();
	~Cylinder();
};


