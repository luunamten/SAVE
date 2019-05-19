#pragma once
#include <GLFW\glfw3.h>
#include <stack>
#include "Cylinder.h"

struct arg {
	short a, b, c, d, num, sizem1, numm1, anchor;
};

class Controller
{
private:
	Controller();
	static float cmY;
	static float cmZ;
	static double offsetX;
	static double offsetY;
	static bool isRun;
	static std::stack<arg> args;
	static Cylinder *cyls[3];
public:
	static void keyCallback(GLFWwindow *window, int code, int scan, int action, int mods);
	static void cursorCallback(GLFWwindow *window, double x, double y);
	static void mouseCallback(GLFWwindow *window, int but, int action, int mods);
	static void scrollCallback(GLFWwindow *window, double x, double y);
	static void reshape(GLFWwindow *window, int nw, int nh);
	static void HNTower();
	static void setRun(bool flag);
	static void HNTowerInit(int numDisks);
	static void setCylinders(Cylinder *cyl1, Cylinder *cyl2, Cylinder *cyl3);
	~Controller();
};

