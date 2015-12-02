#version 330 core

// Vertex Layout
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 norm;


// Scene Matrixes
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
	// Mesh's vertex position
	gl_Position = projection * view * model * vec4(position, 1.0f);

	norm = normalMatrix * normal; 
}
