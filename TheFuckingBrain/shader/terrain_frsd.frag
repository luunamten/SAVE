#version 330 core

in VERTEX {
	vec3 oVCoord;
	vec3 oVNormal;
} frsd;

struct light {
	vec3 direction;
	vec3 color;
	float intensity;	
};

out vec4 oColor;
uniform light lit;
uniform vec3 cameraPosition;
uniform sampler2D smp;
uniform float mapSize;

void main() {
	vec2 coord = vec2(frsd.oVCoord.x/mapSize, frsd.oVCoord.z/mapSize);
	vec3 kd = vec3(texture(smp, coord));
	vec3 ka = vec3(0.3, 0.3, 0.3);
	vec3 ks = kd * 0.5;
	vec3 nVCoordToCamPos = normalize(cameraPosition - frsd.oVCoord);
	vec3 refLitDirection = reflect(lit.direction, frsd.oVNormal);
	vec3 ambient = vec3(0.3, 0.3, 0.3) * kd;
	vec3 diffuse = max(dot(frsd.oVNormal, -lit.direction), 0.0) * lit.intensity * lit.color * kd;
	//vec3 specular = pow(max(dot(refLitDirection, nVCoordToCamPos), 0.0), 20) * lit.intensity * lit.color * vec3(1.0);
	oColor = vec4(ambient + diffuse, 1.0);
}