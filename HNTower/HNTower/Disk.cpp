#include "Disk.h"
#include <GL/glew.h>
#include <vector>
#include <cmath>
#include <glm\gtc\matrix_transform.hpp>
#include "Tool.h"
#include "CommonMatrix.h"
#include "GLContext.h"

int Disk::program = -1;
const float Disk::STEP = 0.005f;

Disk::Disk(std::string &src, float x, float y, float z)
{	
	this->vao = 0;
	this->numVers = 0;
	this->x = x;
	this->y = y;
	this->z = z;
	tgIndex = 3;
	init(src);
}


Disk::~Disk()
{
}

void Disk::initProgram() {
	int vtsd = glCreateShader(GL_VERTEX_SHADER);
	int frsd = glCreateShader(GL_FRAGMENT_SHADER);
	const char * const vtc[] = {
		"#version 440 core\n"
		"layout(location = 0) in vec4 ver;"
		"layout(location = 1) in vec3 inNorm;"
		"layout(location = 0) uniform mat4 pv;"
		"layout(location = 1) uniform mat4 m;"
		"layout(location = 2) uniform mat4 lightMat;"
		"out VT_INTERFACE {"
		"	vec3 norm;"
		"	vec4 shadowCoord;"
		"	vec3 lightVec;"
		"} vertex;"
		"const vec3 light = vec3(-6, 5, 0);"
		"void main() {"
		"	vec4 tmp = m * ver;"
		"	vertex.norm = inNorm;"
		"	vertex.lightVec = normalize(light - tmp.xyz);"
		"	vertex.shadowCoord = lightMat * tmp;"
		"	gl_Position = pv * tmp;"
		"}"
	};
	const char * const frc[] = {
		"#version 440 core\n"
		"in VT_INTERFACE {"
		"	vec3 norm;"
		"	vec4 shadowCoord;"
		"	vec3 lightVec;"
		"} frag;"
		"out vec4 color;"
		"uniform sampler2DShadow sd;"
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
		"	color = vec4(min(vec3(1, 0, 1) * (diffuse * f + ambient), vec3(1.0)), 1.0);"
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

void Disk::init(std::string &src) {
	std::vector<float> vertices;
	std::vector<float> normals;
	unsigned vbo;
	Tool::getSingleModel(src, vertices, normals, numVers);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numVers * 24, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVers * 12, vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, numVers * 12, numVers * 12, normals.data());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12, (void *)(numVers * 12));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Disk::show(bool isDrawToFB) {
	glBindVertexArray(vao);
	if (!isDrawToFB) {
		glUseProgram(program);
		glm::mat4 mat = CommonMatrix::getProjectionMat()
			* CommonMatrix::getLookatMat()
			* CommonMatrix::getRotateMat();
		glm::mat4 modelMat = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
		glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&mat);
		glUniformMatrix4fv(1, 1, GL_FALSE, (float*)&modelMat);
		glUniformMatrix4fv(
			2, 1, GL_FALSE, (float*)&GLContext::getFBToLightMat()
		);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GLContext::getFBTex());
		glDrawArrays(GL_TRIANGLES, 0, numVers);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
	else {
		move();
		glm::mat4 mat =
			GLContext::getFBLightMat()
			* glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
		glUseProgram(GLContext::getFBProgram());
		glUniformMatrix4fv(
			0, 1, GL_FALSE, (float*)&mat
		);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(6.f, 1.f);
		glDrawArrays(GL_TRIANGLES, 0, numVers);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glUseProgram(0);
	}
	glBindVertexArray(0);
}

void Disk::setTarget(float x1, float y1, float x2, float y2, float x3, float y3) {
	tgIndex = 0;
	target[0][0] = x1;
	target[0][1] = y1;
	target[1][0] = x2;
	target[1][1] = y2;
	target[2][0] = x3;
	target[2][1] = y3;
	
}

void Disk::move() {
	if (tgIndex < 3) {
		float tgX = target[tgIndex][0];
		float tgY = target[tgIndex][1];
			float dx = tgX - x;
			float dy = tgY - y;
			float absDx = std::abs(dx);
			float absDy = std::abs(dy);
			if (absDx >= STEP) {
				dx = dx / absDx;
				x += STEP * dx;
			}
			else {
				x = tgX;
			}
			if (absDy >= STEP) {
				dy = dy / absDy;
				y += STEP * dy;
			}
			else{
				y = tgY;
			}
		if(x == tgX && y == tgY){
			tgIndex += 1;
			if (tgIndex == 3) {
				Controller::setRun(true);
			}
		}
	}
}

Disk * Disk::makeNew(std::string src, float x, float y, float z) {
	if (program < 0) {
		initProgram();
	}
	return new Disk(src, x, y, z);
}
