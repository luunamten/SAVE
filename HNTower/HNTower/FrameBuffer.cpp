#include "FrameBuffer.h"
#include <GL\glew.h>
#include "Tool.h"
#include "CommonMatrix.h"
#include <iostream>

int FrameBuffer::program = -1;

FrameBuffer::FrameBuffer(
	float w, float h, glm::mat4 lookatMat, glm::mat4 perspectiveMat
) {
	if (program < 0) {
		initProgram();
	}
	this->w = w;
	this->h = h;
	this->lookatMat = lookatMat;
	this->perspectiveMat = perspectiveMat;
	init();
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::initProgram() {
	const char * const vtc[] = {
		"#version 440 core\n"
		"layout(location = 0) in vec4 ver;"
		"layout(location = 0) uniform mat4 mvp;"
		"void main() {"
		"	gl_Position = mvp * ver;"
		"}"
	};
	const char * const frc[] = {
		"#version 440 core\n"
		"layout(location = 0) out vec4 color;"
		"void main() {"
		"	color = vec4(1.f);"
		"}"
	};
	int vtsd = glCreateShader(GL_VERTEX_SHADER);
	int frsd = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	glShaderSource(vtsd, 1, vtc, nullptr);
	glShaderSource(frsd, 1, frc, nullptr);
	glCompileShader(vtsd);
	Tool::checkStatus(vtsd);
	glCompileShader(frsd);
	Tool::checkStatus(frsd);
	glAttachShader(program, vtsd);
	glAttachShader(program, frsd);
	glLinkProgram(program);
	glDetachShader(program, vtsd);
	glDetachShader(program, frsd);
	glDeleteShader(vtsd);
	glDeleteShader(frsd);
}

void FrameBuffer::init() {
	glGenTextures(1, &tex);
	glGenFramebuffers(1, &fbo);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, 
		GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned FrameBuffer::getFBO() const {
	return fbo;
}

unsigned FrameBuffer::getTex() const {
	return tex;
}

float FrameBuffer::getH() const {
	return h;
}

float FrameBuffer::getW() const {
	return w;
}

glm::mat4 FrameBuffer::getToLigthMat() const {
	glm::mat4 mat(
		glm::vec4(0.5f, 0.f, 0.f, 0.f),
		glm::vec4(0.f, 0.5f, 0.f, 0.f),
		glm::vec4(0.f, 0.f, 0.5f, 0.f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.f)
	);
	return mat * perspectiveMat * lookatMat;
}

glm::mat4 FrameBuffer::getLightMat() const {
	return perspectiveMat * lookatMat;
}

int FrameBuffer::getProgram() {
	return program;
}
