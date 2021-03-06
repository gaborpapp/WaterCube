#version 460 core

layout(location = 0) in int vertexID;
out vec3 vNormal;
out vec3 vPosition;
out float vPressure;

struct Vertex {
	vec4 position;
	vec4 normal;
};

layout(std430, binding = 0) restrict readonly buffer Vertices {
	Vertex vertices[];
};

layout(binding = 1) uniform sampler3D pressureField;
uniform mat4 ciModelViewProjection;
uniform float size;

void main() {
    Vertex v = vertices[vertexID];
	gl_Position = ciModelViewProjection * vec4(v.position.xyz, 1);
	vNormal = v.normal.xyz;
	vPosition = v.position.xyz;
	vPressure = texture(pressureField, vPosition / size).r;
}
