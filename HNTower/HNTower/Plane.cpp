#include "Plane.h"
#include <GL/glew.h>
#include "GLContext.h"
#include <vector>
#include <stdexcept>
#include <soil\SOIL.h>
#include <glm\gtc\matrix_transform.hpp>
#include "Tool.h"
#include "CommonMatrix.h"
#include "FrameBuffer.h"

int Plane::program = -1;

Plane::Plane(std::string &src, float x, float y, float z)
{
	this->vao = 0;
	this->numVers = 0;
	this->x = x;
	this->y = y;
	this->z = z;
	init(src);
}

Plane::~Plane()
{
}

void Plane::initProgram() {
	int vtsd = glCreateShader(GL_VERTEX_SHADER);
	int frsd = glCreateShader(GL_FRAGMENT_SHADER);
	const char * const vtc[] = {
		"#version 440 core\n"
		"layout(location = 0) in vec4 ver;"
		"layout(location = 1) in vec3 inNorm;"
		"layout(location = 2) in vec2 inCoord;"
		"layout(location = 0) uniform mat4 pvm;"
		"layout(location = 1) uniform mat4 m;"
		"layout(location = 2) uniform mat4 lightMat;"
		"out VT_INTERFACE {"
		"	vec3 norm;"
		"	vec2 coord;"
		"	vec4 shadowCoord;"
		"	vec3 lightVec;"
		"} vertex;"
		"const vec3 light = vec3(-6, 5, 0);"
		"void main() {"
		"	vec4 tmp = m * ver;"
		"	vertex.norm = inNorm;"
		"	vertex.coord = inCoord;"
		"	vertex.shadowCoord = lightMat * tmp;"
		"	vertex.lightVec = normalize(light - tmp.xyz);"
		"	gl_Position = pvm * tmp;"
		"}"
	};
	const char * const frc[] = {
		"#version 440 core\n"
		"in VT_INTERFACE {"
		"	vec3 norm;"
		"	vec2 coord;"
		"	vec4 shadowCoord;"
		"	vec3 lightVec;"
		"} frag;"
		"out vec4 color;"
		"layout(location = 3) uniform sampler2D sp;"
		"layout(location = 4) uniform sampler2DShadow sd;"
		"float getFactor(float x, float y) {"
		"	float w = 0.001;"
		"	vec4 offset = vec4(x * w," 
		"		y * w, 0.0, 0.0);"
		"	return textureProj(sd, frag.shadowCoord + offset);"
		"}"
		"void main() {"
		"	float ambient = 0.2;"
		"	float diffuse = max(dot(frag.lightVec, frag.norm), 0.0);"
		"	float f = 0.0;"
		"	for(float x = -2.0; x <= 2.0; x += 2.0) {"
		"		for(float y = -2.0; y <= 2.0; y += 2.0) {"
		"			f += getFactor(x, y);"
		"		}"
		"	}"
		"	f = f * 0.11;"
		"	vec3 tmp = texture(sp, frag.coord).rgb;"
		"	color = vec4(min(tmp * (diffuse * f + ambient), vec3(1.0)), 1.0);"
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

void Plane::init(std::string &src) {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> coords;
	unsigned vbo;
	unsigned char *data = nullptr;
	int w, h, chanels;
	Tool::getSingleModel(src, vertices, normals, coords, numVers);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenTextures(1, &tex);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numVers * 32, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVers * 12, vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, numVers * 12, numVers * 12, normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, numVers * 24, numVers * 8, coords.data());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12, (void *)(numVers * 12));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8, (void *)(numVers * 24));
	glEnableVertexAttribArray(2);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	data = SOIL_load_image("plane2.jpg", &w, &h, &chanels, SOIL_LOAD_RGB);
	if (!data) {
		throw std::runtime_error("Load image error in Plane.");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Plane::show(bool isDrawToFB) {
	glBindVertexArray(vao);
	if (!isDrawToFB) {
		glUseProgram(program);
		glm::mat4 mat = CommonMatrix::getProjectionMat()
			* CommonMatrix::getLookatMat()
			* CommonMatrix::getRotateMat();
		glm::mat4 modelMat = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
		glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&mat);
		glUniformMatrix4fv(1, 1, GL_FALSE, (float*)&modelMat);
		glUniformMatrix4fv(2, 1, GL_FALSE, (float*)&GLContext::getFBToLightMat());
		glUniform1i(3, 0);
		glUniform1i(4, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GLContext::getFBTex());
		glDrawArrays(GL_TRIANGLES, 0, numVers);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
	else {
		glUseProgram(GLContext::getFBProgram());
		glUniformMatrix4fv(
			0, 1, GL_FALSE, (float*)&GLContext::getFBLightMat()
		);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(3.f, 1.f);
		glDrawArrays(GL_TRIANGLES, 0, numVers);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glUseProgram(0);
	}
	glBindVertexArray(0);
}

Plane * Plane::makeNew(
	std::string src, float x, float y, float z) {
	if (program < 0) {
		initProgram();
	}
	return new Plane(src, x, y, z);
}

