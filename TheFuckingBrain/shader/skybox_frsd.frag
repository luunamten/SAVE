#version 330 core

in vec3 oVTexCoord;
out vec4 oColor;
uniform samplerCube sampler;

void main() {
	oColor = texture(sampler, oVTexCoord);
}