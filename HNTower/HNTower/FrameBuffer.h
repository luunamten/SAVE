#pragma once
#include <glm\gtc\matrix_transform.hpp>
class FrameBuffer
{
private:
	static int program;
	unsigned fbo;
	unsigned tex;
	float w;
	float h;
	void init();
	static void initProgram();
	glm::mat4 lookatMat;
	glm::mat4 perspectiveMat;
public:
	FrameBuffer(
		float w, float h, glm::mat4 lookatMat, glm::mat4 perspectiveMat
	);
	~FrameBuffer();
	unsigned getFBO() const;
	unsigned getTex() const;
	float getW() const;
	float getH() const;
	glm::mat4 getToLigthMat() const;
	glm::mat4 getLightMat() const;
	static int getProgram();
};

