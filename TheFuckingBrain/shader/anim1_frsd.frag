#version 330 core

in VERTEX {
	vec3 oVCoord;
	vec3 oVNormal;
	vec3 oka;
	vec3 okd;
	vec3 oks;
} frsd;

struct light {
	vec3 direction;
	vec3 color;
	float intensity;	
};

out vec4 oColor;
uniform light lit;
uniform vec3 cameraPosition;

void main() {
	vec3 normalizedNormal = normalize(frsd.oVNormal);
	vec3 nVCoordToCamPos = normalize(cameraPosition - frsd.oVCoord);
	vec3 refLitDirection = reflect(lit.direction, normalizedNormal);
	vec3 ambient = vec3(0.5) * frsd.okd;
	vec3 diffuse = max(dot(normalizedNormal, -lit.direction), 0.0) * lit.intensity * lit.color * frsd.okd;
	//vec3 specular = pow(max(dot(refLitDirection, nVCoordToCamPos), 0.0), 5) * lit.intensity * lit.color * frsd.oks;
	oColor = vec4(ambient + diffuse, 1.0);
}