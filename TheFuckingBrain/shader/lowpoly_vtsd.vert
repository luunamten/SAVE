#version 330 core

in vec4 iVCoord;
in vec3 iVNormal;
in vec3 ika;
in vec3 ikd;
in vec3 iks;

out VERTEX {
	vec3 oVCoord;
	vec3 oVNormal;
	vec3 oka;
	vec3 okd;
	vec3 oks;
} vtsd;

uniform mat4 projViewModelMat;

void main() {
	vtsd.oVCoord = vec3(iVCoord);
	vtsd.oVNormal = iVNormal;
	vtsd.oka = ika;
	vtsd.okd = ikd;
	vtsd.oks = iks;
	gl_Position = projViewModelMat * iVCoord;
}
