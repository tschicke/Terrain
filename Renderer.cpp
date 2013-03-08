/*
 * Renderer.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Tyler
 */

#include <gl/glew.h>
#include <glm/gtx/transform.hpp>

#include "Renderer.h"

#include <iostream>

using namespace glm;

mat4 Renderer::projectionMatrix;

Renderer& Renderer::getMainRenderer(){
	static Renderer renderer;
	return renderer;
}

Renderer::Renderer() {
	setProjectionMatrix(90, 600, 400, 0.1, 100);
}

void Renderer::createMesh(MeshID * meshID) {
	glGenBuffers(1, meshID->getVertexIDAddress());
	glGenBuffers(1, meshID->getIndexIDAddress());
	currentMeshID = *meshID;
}

void Renderer::endMesh(MeshID * meshID) {
	if (currentMeshID == *meshID) {
		glBindBuffer(GL_ARRAY_BUFFER, meshID->getVertexID());
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * currentData.size(), &(currentData[0]), GL_STATIC_DRAW); //Should be STATIC_DRAW??
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshID->getIndexID());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * currentIndices.size(), &(currentIndices[0]), GL_STATIC_DRAW); //Should be STATIC_DRAW??
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		meshID->setNumVertices(currentIndices.size());

		/* Print data in current data with labels
		for (unsigned int i = 0; i < currentData.size(); i++) {
			float data = currentData.at(i);
			float counter = i / 3.f;

			if (fmodf(counter, 3) == 0) {
				std::cout << "\nvertex: ";
			}

			if (fmodf(counter - 1, 3) == 0) {
				std::cout << "\ncolor: ";
			}

			if (fmodf(counter - 2, 3) == 0) {
				std::cout << "\nnormal: ";
			}
			std::cout << data << ' ';
		}*/

		currentMeshID.reset();
		currentData.clear();
		currentIndices.clear();
	}
}

int Renderer::addVertexToMesh(MeshID meshID, vec3 position, vec3 normal, float r, float g, float b) {
	if (currentMeshID == meshID && meshID.getColorType()  == Color) {
		int vertexIndex = std::distance(currentData.begin(), currentData.end()) / 9; // /9 temp

		//Add Vertex
		currentData.push_back(position.x);
		currentData.push_back(position.y);
		currentData.push_back(position.z);

		//Add Color
		currentData.push_back(r);
		currentData.push_back(g);
		currentData.push_back(b);

		//Add Normal
		currentData.push_back(normal.x);
		currentData.push_back(normal.y);
		currentData.push_back(normal.z);

		return vertexIndex;

		//TODO Possibly check for duplicate vertices??
	}

	return -1;
}

int Renderer::addVertexToMesh(MeshID meshID, vec3 position, vec3 normal, float u, float v) {
	if (currentMeshID == meshID && meshID.getColorType() == Texture) {
		int vertexIndex = std::distance(currentData.begin(), currentData.end()) / 8; // /9 temp

		//Add Vertex
		currentData.push_back(position.x);
		currentData.push_back(position.y);
		currentData.push_back(position.z);

		//Add Texture Coordinates
		currentData.push_back(u);
		currentData.push_back(v);

		//Add Normal
		currentData.push_back(normal.x);
		currentData.push_back(normal.y);
		currentData.push_back(normal.z);

		return vertexIndex;

		//TODO Possibly check for duplicate vertices??
	}

	return -1;
}

void Renderer::addTriangleToMesh(MeshID meshID, int v1, int v2, int v3) {
	if (currentMeshID == meshID) {
		currentIndices.push_back(v1);
		currentIndices.push_back(v2);
		currentIndices.push_back(v3);
	}
}

void Renderer::deleteMesh(MeshID * meshID) {
	glDeleteBuffers(1, meshID->getVertexIDAddress());
	glDeleteBuffers(1, meshID->getIndexIDAddress());
}

void Renderer::renderMesh(MeshID meshID) {
	glBindBuffer(GL_ARRAY_BUFFER, meshID.getVertexID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshID.getIndexID());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	int vertexSize = 3;
	int colorSize = (meshID.getColorType() == Texture ? 2 : 3);
	int normalSize = 3;
	int stride = vertexSize + colorSize + normalSize;

	glVertexAttribPointer(0, vertexSize, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void *) 0);
	glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void *) (3 * sizeof(float)));
	glVertexAttribPointer(2, vertexSize, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void *) ((3 + colorSize) * sizeof(float)));

	glDrawElements(GL_TRIANGLES, meshID.getNumVertices(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::setProjectionMatrix(float fov, float width, float height, float zNear, float zFar){
	projectionMatrix = perspective(fov, width / height, zNear, zFar);
}

glm::mat4* Renderer::getProjectionMatrix(){
	return &projectionMatrix;
}

MeshID::MeshID() {
	reset();
}

MeshID::MeshID(ColorType t){
	reset();
	type = t;
}

MeshID::~MeshID() {
}

void MeshID::reset(){
	vertexID = 0;
	indexID = 0;
	numOfVertices = 0;
	type = Default;
}

uint* MeshID::getVertexIDAddress(){
	return &vertexID;
}

uint* MeshID::getIndexIDAddress(){
	return &indexID;
}

uint MeshID::getVertexID(){
	return vertexID;
}

uint MeshID::getIndexID(){
	return indexID;
}

void MeshID::setNumVertices(uint numV){
	numOfVertices = numV;
}

uint MeshID::getNumVertices(){
	return numOfVertices;
}

bool MeshID::setColorType(ColorType t){
	if(numOfVertices == 0){
		type = t;
		return true;
	}
	return false;
}

ColorType MeshID::getColorType(){
	return type;
}

void MeshID::operator =(MeshID meshID){
	vertexID = meshID.vertexID;
	indexID = meshID.indexID;
	numOfVertices = meshID.numOfVertices;
	type = meshID.getColorType();
}

bool MeshID::operator ==(MeshID meshID){
	if(meshID.vertexID != vertexID)return false;
	if(meshID.indexID != indexID)return false;
	if(meshID.numOfVertices != numOfVertices)return false;
	if(meshID.getColorType() != type)return false;

	return true;
}

