#version 330

uniform mat4 modelMatrix, viewMatrix, projectionMatrix;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;

out vec3 normal;
out vec3 color;

void main(){
	color = vertexColor;
	normal = vertexNormal;
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1);
}