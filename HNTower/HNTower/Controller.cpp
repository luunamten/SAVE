#include <GL\glew.h>
#include "Controller.h"
#include "CommonMatrix.h"
#include "GLContext.h"

float Controller::cmY = 0.3f;
float Controller::cmZ = 1.f;
double Controller::offsetX = -1;
double Controller::offsetY = -1;
bool Controller::isRun = true;
std::stack<arg> Controller::args;
Cylinder *Controller::cyls[3];

Controller::Controller()
{
}


Controller::~Controller()
{
}

void Controller::keyCallback(GLFWwindow *window, int code, int scan, int action, int mods) {
	if (code == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Controller::cursorCallback(GLFWwindow *window, double x, double y) {
	if (offsetX >= 0) {
		double dx = (x - offsetX) / 5;
		double dy = (y - offsetY) / 5;
		offsetX = x;
		offsetY = y;
		CommonMatrix::rotate(dy, dx);
	}
}

void Controller::mouseCallback(GLFWwindow *window, int but, int action, int mods) {
	if (but == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &offsetX, &offsetY);
		}
		else if (action == GLFW_RELEASE) {
			offsetX = -1;
		}
	}
}

void Controller::scrollCallback(GLFWwindow *window, double x, double y) {
	if (y >= 0 && cmY >= 0.05f && cmZ >= 0.05f) {
		cmY /= 1.2f;
		cmZ /= 1.2f;
		CommonMatrix::lookat(0, cmY, cmZ, 0, 0.2f, 0, 0, 1, 0);
	}
	else if(y < 0 && cmY < 10.f && cmZ <= 10.f) {
		cmY *= 1.2f;
		cmZ *= 1.2f;
		CommonMatrix::lookat(0, cmY, cmZ, 0, 0.2f, 0, 0, 1, 0);
	}
}

void Controller::reshape(GLFWwindow *window, int nw, int nh) {
	CommonMatrix::projection(60, nw * 1.f / nh, 0.017f, 1000);
	GLContext::setViewportSize(nw, nh);
}

void Controller::setRun(bool flag) {
	isRun = flag;
}

void Controller::setCylinders(
	Cylinder *cyl1, Cylinder *cyl2, Cylinder *cyl3
) {
	cyls[0] = cyl1;
	cyls[1] = cyl2;
	cyls[2] = cyl3;
}

void Controller::HNTowerInit(int numDisks) {
	arg tmp;
	tmp.a = 0;
	tmp.b = 1;
	tmp.c = 2;
	tmp.d = 0;
	tmp.num = numDisks;
	tmp.sizem1 = numDisks - 1;
	tmp.numm1 = tmp.num - 1;
	tmp.anchor = 1;
	args.push(tmp);
}

void Controller::HNTower() {
	if (isRun) {
		if (args.size() == 0) {
			isRun = false;
			return;
		}
		for (; args.size() > 0;) {
			arg &topE = args.top();
			if (topE.anchor == 1) {
				topE.anchor = 2;
				if (topE.d < topE.sizem1) {
					arg tmp;
					tmp.a = topE.a;
					tmp.b = topE.c;
					tmp.c = topE.b;
					tmp.d = topE.d + 1;
					tmp.num = topE.numm1;
					tmp.sizem1 = topE.sizem1;
					tmp.numm1 = tmp.num - 1;
					tmp.anchor = 1;
					args.push(tmp);
					continue;
				}
			}
			if (topE.anchor == 2) {
				Cylinder *tgCyl = cyls[topE.c];
				Cylinder *srcCyl = cyls[topE.a];
				Disk *topDisk = srcCyl->top();
				float tgX = tgCyl->getX();
				tgCyl->push(topDisk);
				srcCyl->pop();
				topDisk->setTarget(
					srcCyl->getX(), 0.22f, tgX, 0.22f, tgX, tgCyl->getTopPos()
				);
				isRun = false;
				if (topE.d < topE.sizem1) {
					arg tmp;
					tmp.a = topE.b;
					tmp.b = topE.a;
					tmp.c = topE.c;
					tmp.d = cyls[tmp.a]->size() - topE.numm1;
					tmp.num = topE.numm1;
					tmp.sizem1 = cyls[tmp.a]->size() - 1;
					tmp.numm1 = tmp.num - 1;
					tmp.anchor = 1;
					topE.anchor = 3;
					args.push(tmp);
				}
				else {
					args.pop();
				}
				return;
			}
			else {
				args.pop();
			}
		}
	}
}
