/*
 * TerrainChunk.cpp
 *
 *  Created on: Mar 7, 2013
 *      Author: Tyler
 */

#include "TerrainChunk.h"

#include <gl/glew.h>//Temp?
#include <glm/gtx/transform.hpp>
using namespace glm;

const int chunkSize = 32;

TerrainChunk::TerrainChunk() {
	init(vec3(0, 0, 0));
}

TerrainChunk::TerrainChunk(vec3 pos) {
	init(pos);
}

void TerrainChunk::init(vec3 pos) {
	position = pos;
	coordinates = new vec3[chunkSize * chunkSize];
	meshID = MeshID(Color);
	create();
}

TerrainChunk::~TerrainChunk() {
}

void TerrainChunk::create() {
	Renderer::getMainRenderer().createMesh(&meshID);

	int v1 = Renderer::getMainRenderer().addVertexToMesh(meshID, vec3(0, 0, 1), vec3(0, 0, 1), 1,
			0, 0);
	int v2 = Renderer::getMainRenderer().addVertexToMesh(meshID, vec3(1, 0, 1), vec3(0, 0, 1), 1,
			1, 0);
	int v3 = Renderer::getMainRenderer().addVertexToMesh(meshID, vec3(1, 1, 1), vec3(0, 0, 1), 1,
			0, 1);

	Renderer::getMainRenderer().addTriangleToMesh(meshID, v1, v2, v3);

//	for(int x = 0; x < chunkSize; x++){
//		for (int z = 0; z < chunkSize; z++){
//			int y =
//		}
//	}

	Renderer::getMainRenderer().endMesh(&meshID);

	Shader vertShader, fragShader;
	vertShader.loadShader("shaders/vertexShader.vert", GL_VERTEX_SHADER);
	fragShader.loadShader("shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);

	program.createProgram();
	program.addShader(&vertShader);
	program.addShader(&fragShader);
	program.linkProgram();

	vertShader.deleteShader();
	fragShader.deleteShader();
}

void TerrainChunk::draw() {
	program.useProgram();

	mat4 modelMatrix = mat4(1.0f);
	mat4 viewMatrix = lookAt(position, vec3(0.f, 0.f, 1.f), vec3(0.f, 1.f, 0.f));

	program.setUniform("modelMatrix", &modelMatrix, 1);
	program.setUniform("viewMatrix", &viewMatrix, 1);
	program.setUniform("projectionMatrix", Renderer::getProjectionMatrix(), 1);

	Renderer::getMainRenderer().renderMesh(meshID);

}

