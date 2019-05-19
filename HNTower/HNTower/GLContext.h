#pragma once
#include <GLFW\glfw3.h>
#include <memory>
#include "Cylinder.h"
#include "Disk.h"
#include "Plane.h"
#include "Controller.h"
#include "SkyBox.h"
#include "FrameBuffer.h"
#include "Rect.h"

class GLContext
{
private:
	static GLFWwindow *window;
	static float w;
	static float h;
	static std::unique_ptr<Cylinder> cyl1;
	static std::unique_ptr<Cylinder> cyl2;
	static std::unique_ptr<Cylinder> cyl3;
	static std::unique_ptr<Plane> plane;
	static std::unique_ptr<SkyBox> skybox;
	static std::unique_ptr<Disk> dsk1;
	static std::unique_ptr<Disk> dsk2;
	static std::unique_ptr<Disk> dsk3;
	static std::unique_ptr<Disk> dsk4;
	static std::unique_ptr<Disk> dsk5;
	static std::unique_ptr<Disk> dsk6;
	static std::unique_ptr<FrameBuffer> framebuffer;
	static std::unique_ptr<Rect> rect;
	static Controller controller;
	static void init();
	static void draw();
	static void dispose();
	GLContext();
public:
	static void initContext();
	static void GLContext::setViewportSize(float nw, float nh);
	static const unsigned getFBTex();
	static const glm::mat4 getFBToLightMat();
	static const glm::mat4 getFBLightMat();
	static const int getFBProgram();
	~GLContext();
};

