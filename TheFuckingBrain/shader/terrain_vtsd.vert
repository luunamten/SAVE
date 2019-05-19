#version 330 core

in vec4 iVCoord;
in vec3 iVNormal;

out VERTEX {
	vec3 oVCoord;
	vec3 oVNormal;
} vtsd;

uniform mat4 projViewModelMat;

void main() {
	vtsd.oVCoord = vec3(iVCoord);
	vtsd.oVNormal = iVNormal;
	gl_Position = projViewModelMat * iVCoord;
}
