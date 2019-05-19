#pragma once
#include <glm\gtc\matrix_transform.hpp>

class CommonMatrix
{
private:
	static glm::mat4 projectionMat;
	static glm::mat4 lookatMat;
	static glm::mat4 frustrumMat;
	static glm::mat4 rotateMat;
	static glm::mat4 ortho2DMat;
	CommonMatrix();
public:
	static void projection(float angle, float ratio, float near, float far);
	static void frustrum(
		float nw, float pw, float nh, float ph, float near, float far
	);
	static void lookat(float ex, float ey, float ez,
		float ax, float ay, float az,
		float ux, float uy, float uz);
	static void rotate(double angX, double angY);
	static void ortho2D(float nw, float pw, float nh, float ph);
	static const glm::mat4 & getProjectionMat();
	static const glm::mat4 & getLookatMat();
	static const glm::mat4 & getFrustrumMat();
	static const glm::mat4 & getRotateMat();
	static const glm::mat4 & getOrtho2DMat();
	~CommonMatrix();
};

