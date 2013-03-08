/*
 * TerrainChunk.h
 *
 *  Created on: Mar 7, 2013
 *      Author: Tyler
 */

#ifndef TERRAINCHUNK_H_
#define TERRAINCHUNK_H_

#include <glm/glm.hpp>

#include "Renderer.h"
#include "Shader.h"

class TerrainChunk {
public:
	TerrainChunk();
	TerrainChunk(glm::vec3 position);
	virtual ~TerrainChunk();

	void init(glm::vec3 position);

	void draw();

private:
	glm::vec3 position;
	glm::vec3 *coordinates;

	MeshID meshID;
	ShaderProgram program;

	void create();
};

#endif /* TERRAINCHUNK_H_ */
