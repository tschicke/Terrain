/*
 * main.cpp
 *
 *  Created on: Mar 7, 2013
 *      Author: Tyler
 */

#include <iostream>

#include <gl/glew.h>
#include <SFML/Window.hpp>

#include "TerrainChunk.h"

int main(){
	sf::Window window;
	window.create(sf::VideoMode(800, 600, 32), "Terrain");

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cerr << "Glew did not initialize properly" << std::endl;
	}

	TerrainChunk chunk;

	bool running = true;
	while (running){
		sf::Event event;
		while(window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				running = false;
			}
		}

		chunk.draw();

		window.display();
	}

	return 0;
}


