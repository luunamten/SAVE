#version 330 core

in vec4 iVCoord;
in vec3 iVNormal;
in vec2 iVTexCoord;

uniform mat4 projViewModelMat;

out vec3 oVNormal;
out vec2 oVTexCoord;
out vec3 oVCoord;

void main() {
	oVNormal = iVNormal;
	oVTexCoord = iVTexCoord;
	oVCoord = vec3(iVCoord);
	gl_Position = projViewModelMat * iVCoord;
}