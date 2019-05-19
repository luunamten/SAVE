#include "SkyBox.h"
#include <GL/glew.h>
#include <vector>
#include <stdexcept>
#include <soil\SOIL.h>
#include <glm\gtc\matrix_transform.hpp>
#include "Tool.h"
#include "CommonMatrix.h"

int SkyBox::program = -1;

SkyBox::SkyBox(float x, float y, float z)
{
	init();
}

SkyBox::~SkyBox()
{
}

void SkyBox::initProgram() {
	int vtsd = glCreateShader(GL_VERTEX_SHADER);
	int frsd = glCreateShader(GL_FRAGMENT_SHADER);
	const char * const vtc[] = {
		"#version 440 core\n"
		"layout(location = 0) in vec4 ver;"
		"layout(location = 1) in vec3 inCoord;"
		"layout(location = 0) uniform mat4 pvm;"
		"out vec3 coord;"
		"void main() {"
		"	vec4 tmp = pvm * ver;"
		"	tmp.z = tmp.w;"
		"	coord = inCoord;"
		"	gl_Position = tmp;"
		"}"
	};
	const char * const frc[] = {
		"#version 440 core\n"
		"in vec3 coord;"
		"out vec4 color;"
		"uniform samplerCube sp;"
		"void main() {"
		"	color = texture(sp, coord);"
		"}"
	};
	program = glCreateProgram();
	glShaderSource(vtsd, 1, vtc, nullptr);
	glShaderSource(frsd, 1, frc, nullptr);
	glCompileShader(vtsd);
	glCompileShader(frsd);
	glAttachShader(program, vtsd);
	glAttachShader(program, frsd);
	glLinkProgram(program);
	glDetachShader(program, vtsd);
	glDetachShader(program, frsd);
	glDeleteShader(vtsd);
	glDeleteShader(frsd);
}

void SkyBox::init() {
	unsigned vbo;
	const char *imgSrc[6] = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	float vers[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	numVers = sizeof(vers) / 12;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenTextures(1, &tex);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vers), vers, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12, 0);
	glEnableVertexAttribArray(1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	for (int i = 0; i < 6; i++) {
		int w, h, chanels;
		unsigned char *data = SOIL_load_image(imgSrc[i], &w, &h, &chanels, SOIL_LOAD_RGB);
		if (!data) {
			throw std::runtime_error("Load image error in SkyBox.");
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h,
			0, GL_RGB, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SkyBox::show() {
	glUseProgram(program);
	glm::mat4 mat = CommonMatrix::getProjectionMat()
		* CommonMatrix::getLookatMat()
		* CommonMatrix::getRotateMat()
		* glm::translate(glm::mat4(1.f), glm::vec3(x, y, z))
		* glm::scale(glm::mat4(1.f), glm::vec3(100, 100, 100));
	glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&mat);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, numVers);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

SkyBox * SkyBox::makeNew(float x, float y, float z) {
	if (program < 0) {
		initProgram();
	}
	return new SkyBox(x, y, z);
}
