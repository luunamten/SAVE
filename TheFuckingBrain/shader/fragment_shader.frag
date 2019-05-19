#version 330 core

struct light {
	vec3 direction;
	vec3 color;
	float intensity;	
};

in vec3 oVNormal;
in vec3 oVCoord;
in vec2 oVTexCoord;

out vec4 oColor;
uniform sampler2D sampler;
uniform light lit;
uniform vec3 cameraPosition;

void main() {
	vec3 nVCoordToCamPos = normalize(cameraPosition - oVCoord);
	vec3 refLitDirection = reflect(lit.direction, oVNormal);
	vec3 ambient = vec3(0.5, 0.5, 0.5) * lit.color * lit.intensity;
	vec3 diffuse = max(dot(oVNormal, -lit.direction), 0.0) * lit.intensity * lit.color;
	vec3 specular = pow(max(dot(refLitDirection, nVCoordToCamPos), 0.0), 2) * lit.color * lit.intensity;
	vec4 texColor = texture(sampler, oVTexCoord);
	oColor = vec4(texColor.rgb * (ambient + diffuse) + specular, texColor.a);
}