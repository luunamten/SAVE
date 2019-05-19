#version 330 core

in vec4 iVCoord;

uniform mat4 projViewModelMat;

out vec3 oVTexCoord;

void main() {
	oVTexCoord = iVCoord.xyz;
	vec4 pos = projViewModelMat * iVCoord;
	gl_Position = pos.xyww;
}