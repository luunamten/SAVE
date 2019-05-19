#include <GL\glew.h>
#include <soil\SOIL.h>
#include <stdexcept>
#include <vector>
#include <iterator>
#include "CommonMatrix.h"
#include "Tool.h"
#include "Cylinder.h"
#include "GLContext.h"

int Cylinder::program = -1;
const float Cylinder::HEIGHT = 0.02f;

Cylinder::Cylinder(std::string &src, float x, float y, float z)
{
	this->vao = 0;
	this->numVers = 0;
	this->x = x;
	this->y = y;
	this->z = z;
	init(src);
}

Cylinder::~Cylinder()
{
}

void Cylinder::initProgram() {
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
		"	vec4 offset = vec4(x * w, y * w, 0.0, 0.0);"
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
		"	color = vec4(min(vec3(0, 1, 1) * (diffuse * f + ambient), vec3(1.0)), 1.0);"
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
void Cylinder::init(std::string &src) {
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

void Cylinder::show(bool isDrawToFB) {
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
	} else {
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

Cylinder * Cylinder::makeNew(std::string src, float x, float y, float z) {
	if (program < 0) {
		initProgram();
	}
	return new Cylinder(src, x, y, z);
}

void Cylinder::addDisk(Disk *disk) {
	disks.push(disk);
}

void Cylinder::pop() {
	disks.pop();
}

void Cylinder::push(Disk *disk) {
	disks.push(disk);
}

float Cylinder::getX() {
	return x;
}

float Cylinder::getY() {
	return y;
}

float Cylinder::getTopPos() {
	return (disks.size() - 1) * HEIGHT;
}

size_t Cylinder::size() {
	return disks.size();
}

Disk * Cylinder::top() {
	return disks.top();
}