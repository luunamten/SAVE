#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <stdexcept>
#include "GLContext.h"
#include "CommonMatrix.h"

GLFWwindow *GLContext::window = nullptr;
float GLContext::w = 0;
float GLContext::h = 0;
std::unique_ptr<Cylinder> GLContext::cyl1;
std::unique_ptr<Cylinder> GLContext::cyl2;
std::unique_ptr<Cylinder> GLContext::cyl3;
std::unique_ptr<Plane> GLContext::plane;
std::unique_ptr<SkyBox> GLContext::skybox;
std::unique_ptr<Disk> GLContext::dsk1;
std::unique_ptr<Disk> GLContext::dsk2;
std::unique_ptr<Disk> GLContext::dsk3;
std::unique_ptr<Disk> GLContext::dsk4;
std::unique_ptr<Disk> GLContext::dsk5;
std::unique_ptr<Disk> GLContext::dsk6;
std::unique_ptr<FrameBuffer> GLContext::framebuffer;
std::unique_ptr<Rect> GLContext::rect;

GLContext::GLContext()
{
	initContext();
}


GLContext::~GLContext()
{
}


void GLContext::initContext() {
	const GLFWvidmode *vid = nullptr;
	if (!glfwInit()) {
		throw std::runtime_error("Init glfw3 error.");
	}
	vid = glfwGetVideoMode(glfwGetPrimaryMonitor());
	w = vid->width;
	h = vid->height;
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RED_BITS, vid->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, vid->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, vid->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, vid->refreshRate);
	window = glfwCreateWindow(w, h, "HNTower", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("Create window error.");
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		throw std::runtime_error("Init glew error.");
	}
	glewExperimental = true;
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, controller.keyCallback);
	glfwSetCursorPosCallback(window, Controller::cursorCallback);
	glfwSetMouseButtonCallback(window, Controller::mouseCallback);
	glfwSetScrollCallback(window, Controller::scrollCallback);
	glfwSetFramebufferSizeCallback(window, Controller::reshape);
	try {
		init();
	}
	catch (std::runtime_error &exp) {
		glfwDestroyWindow(window);
		glfwTerminate();
		throw exp;
	}
	draw();
	dispose();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLContext::init() {
	glClearColor(0.1, 0.1f, 0.1f, 1.f);
	CommonMatrix::projection(60, w / h, 0.017f, 1000);
	CommonMatrix::lookat(0, 0.3f, 1.f, 0, 0.2f, 0, 0, 1, 0);
	CommonMatrix::ortho2D(-1, 1, -1, 1);
	cyl1 = std::unique_ptr<Cylinder>(
		Cylinder::makeNew("ccylinder.obj", -0.19f, 0, 0)
		);
	cyl2 = std::unique_ptr<Cylinder>(
		Cylinder::makeNew("ccylinder.obj", 0, 0, 0)
		);
	cyl3 = std::unique_ptr<Cylinder>(
		Cylinder::makeNew("ccylinder.obj", 0.19f, 0, 0)
		);
	plane = std::unique_ptr<Plane>(
		Plane::makeNew("plane.obj", 0, 0, 0)
		);
	skybox = std::unique_ptr<SkyBox>(
		SkyBox::makeNew(0, 0, 0)
		);
	dsk1 = std::unique_ptr<Disk>(
		Disk::makeNew("dc.obj", -0.19f, 0, 0)
		);
	cyl1->addDisk(dsk1.get());
	dsk2 = std::unique_ptr<Disk>(
		Disk::makeNew("dc1.obj", -0.19f, 0.02f, 0)
		);
	cyl1->addDisk(dsk2.get());
	dsk3 = std::unique_ptr<Disk>(
		Disk::makeNew("dc2.obj", -0.19f, 0.04f, 0)
		);
	cyl1->addDisk(dsk3.get());
	dsk4 = std::unique_ptr<Disk>(
		Disk::makeNew("dc3.obj", -0.19f, 0.06f, 0)
		);
	cyl1->addDisk(dsk4.get());
	dsk5 = std::unique_ptr<Disk>(
		Disk::makeNew("dc4.obj", -0.19f, 0.08f, 0)
		);
	cyl1->addDisk(dsk5.get());
	dsk6 = std::unique_ptr<Disk>(
		Disk::makeNew("dc5.obj", -0.19f, 0.1f, 0)
		);
	cyl1->addDisk(dsk6.get());
	framebuffer = std::unique_ptr<FrameBuffer>(
		new FrameBuffer(
			w * 4 , h * 4 , glm::lookAt(
				glm::vec3(-6.f, 5.f, 0.f),
				glm::vec3(0.f, 0.f, 0.f),
				glm::vec3(0.f, 1.f, 0.f)
			),
			glm::perspective(
				glm::radians(40.f), w / h, 4.f, 10.f
			)
		)
		);
	rect = std::unique_ptr<Rect>(
		Rect::makeNew(
			0, 0, 0, 2, 2, 
			framebuffer->getTex()
		)
		);
	Controller::setCylinders(cyl1.get(), cyl2.get(), cyl3.get());
	Controller::HNTowerInit(6);
}

void GLContext::draw() {
	while (!glfwWindowShouldClose(window)) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getFBO());
		glViewport(0, 0, framebuffer->getW(), framebuffer->getH());
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.f);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		plane->show(true);
		cyl1->show(true);
		cyl2->show(true);
		cyl3->show(true);
		cyl1->show(true);
		cyl2->show(true);
		cyl3->show(true);
		dsk1->show(true);
		dsk2->show(true);
		dsk3->show(true);
		dsk4->show(true);
		dsk5->show(true);
		dsk6->show(true);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, w, h);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Controller::HNTower();
		plane->show(false);
		cyl1->show(false);
		cyl2->show(false);
		cyl3->show(false);
		dsk1->show(false);
		dsk2->show(false);
		dsk3->show(false);
		dsk4->show(false);
		dsk5->show(false);
		dsk6->show(false);
		skybox->show();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

const unsigned GLContext::getFBTex() {
	return framebuffer->getTex();
}

const glm::mat4 GLContext::getFBToLightMat() {
	return framebuffer->getToLigthMat();
}

const glm::mat4 GLContext::getFBLightMat() {
	return framebuffer->getLightMat();
}

const int GLContext::getFBProgram() {
	return framebuffer->getProgram();
}

void GLContext::setViewportSize(float nw, float nh) {
	w = nw;
	h = nh;
}

void GLContext::dispose() {

}


