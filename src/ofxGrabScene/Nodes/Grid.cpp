//
//  Grid.cpp
//  ofxGrabScene
//
#include "Grid.h"

namespace GrabScene {
	namespace Nodes {
		//----------
		Grid::Grid() {
			this->parameters.add(this->scale);
			this->parameters.add(this->ticks);
			this->parameters.add(this->labels);
			this->parameters.add(this->showX);
			this->parameters.add(this->showY);
			this->parameters.add(this->showZ);
			
			
			this->scale.set("Scale log10", 1.0f, -2.0f, 4.0f);
			this->ticks.set("Ticks", 2, 1, 100);
			this->labels.set("Show labels", true);
			this->showX.set("X", false);
			this->showY.set("Y", true);
			this->showZ.set("Z", false);
			
			this->setName("Grid");
		}
		
		//----------
		void Grid::draw() {
			shader("noLight").begin();
			
			this->transformGL();
			float scale = pow(10.0f, this->scale);
			ofDrawGrid(scale, this->ticks, this->labels, this->showX, this->showY, this->showZ);
			this->restoreTransformGL();
			
			shader("noLight").end();
		}
		
		//----------
		void Grid::drawStencil() {
			this->transformGL();
			float scale = pow(10.0f, float(this->scale));
			ofDrawGrid(scale, this->ticks, false, this->showX, this->showY, this->showZ);
			this->restoreTransformGL();
		}
	}
}