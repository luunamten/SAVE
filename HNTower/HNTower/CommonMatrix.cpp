#include "CommonMatrix.h"

glm::mat4 CommonMatrix::projectionMat;
glm::mat4 CommonMatrix::lookatMat;
glm::mat4 CommonMatrix::frustrumMat;
glm::mat4 CommonMatrix::rotateMat;
glm::mat4 CommonMatrix::ortho2DMat;

CommonMatrix::CommonMatrix()
{
}
CommonMatrix::~CommonMatrix()
{
}

void CommonMatrix::projection(
	float angle, float ratio, float mnear, float mfar
) {
	projectionMat = glm::perspective(
		glm::radians(angle), ratio, mnear, mfar
	);
}

void CommonMatrix::frustrum(
	float nw, float pw, float nh, float ph, float near, float far
) {
	frustrumMat = glm::frustum(
		nw, pw, nh, ph, near, far
	);
}
void CommonMatrix::lookat(
	float ex, float ey, float ez,
	float ax, float ay, float az,
	float ux, float uy, float uz
) {
	lookatMat = glm::lookAt(
		glm::vec3(ex, ey, ez),
		glm::vec3(ax, ay, az),
		glm::vec3(ux, uy, uz)
	);
}

void CommonMatrix::rotate(double angX, double angY) {
	rotateMat = glm::rotate(glm::mat4(1.f), (float)glm::radians(angX), glm::vec3(1, 0, 0))
		* rotateMat
		* glm::rotate(glm::mat4(1.f), (float)glm::radians(angY), glm::vec3(0, 1, 0));
}

void CommonMatrix::ortho2D(float nw, float pw, float nh, float ph) {
	ortho2DMat = glm::ortho(nw, pw, nh, ph);
}

const glm::mat4 & CommonMatrix::getProjectionMat() {
	return projectionMat;
}

const glm::mat4 & CommonMatrix::getLookatMat() {
	return lookatMat;
}

const glm::mat4 & CommonMatrix::getFrustrumMat() {
	return frustrumMat;
}

const glm::mat4 & CommonMatrix::getRotateMat() {
	return rotateMat;
}

const glm::mat4 & CommonMatrix::getOrtho2DMat() {
	return ortho2DMat;
}


