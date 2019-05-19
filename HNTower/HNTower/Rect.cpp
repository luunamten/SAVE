#include "Rect.h"
#include <GL/glew.h>
#include "Tool.h"
#include <glm\gtc\matrix_transform.hpp>
#include "CommonMatrix.h"

int Rect::program = -1;

Rect::Rect(
	float x, float y, float z, float w, float h, unsigned tex
)
{
	this->vao = 0;
	this->w = w;
	this->h = h;
	this->x = x;
	this->y = y;
	this->z = z;
	this->tex = tex;
	init();
}

Rect::~Rect()
{
}

void Rect::initProgram() {
	int vtsd = glCreateShader(GL_VERTEX_SHADER);
	int frsd = glCreateShader(GL_FRAGMENT_SHADER);
	const char * const vtc[] = {
		"#version 440 core\n"
		"layout(location = 0) in vec4 ver;"
		"layout(location = 1) in vec2 inCoord;"
		"layout(location = 0) uniform mat4 pvm;"
		"out vec2 coord;"
		"void main() {"
		"	coord = inCoord;"
		"	gl_Position = pvm * ver;"
		"}"
	};
	const char * const frc[] = {
		"#version 440 core\n"
		"in vec2 coord;"
		"out vec4 color;"
		"uniform sampler2D sp;"
		"void main() {"
		"	color = texture(sp, coord);"
		"}"
	};
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

void Rect::init() {
	unsigned vbo;
	float vers[] = {
		1.f, 1.f,
		-1.f, 1.f,
		-1, -1,
		1, -1,
		1, 1,
		0, 1,
		0, 0,
		1, 0
	};
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vers), vers, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8, (void *)32);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Rect::show() {
	glBindVertexArray(vao);
	glUseProgram(program);
	glUniformMatrix4fv(
		0, 1, GL_FALSE, (float*)&CommonMatrix::getOrtho2DMat()
	);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

Rect * Rect::makeNew(
	float x, float y, float z, float w, float h, unsigned tex
	) {
	if (program < 0) {
		initProgram();
	}
	return new Rect(x, y, z, w, h, tex);
}