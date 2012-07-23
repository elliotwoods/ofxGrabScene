//
//  Grid.h
//  ofxGrabScene
//
#pragma once

#include "../Node.h"
#include "../Assets.h"

namespace GrabScene {
	namespace Nodes {
		class Grid : public Node {
		public:
			Grid();
			void draw();
			void drawStencil();
			
		protected:
			ofParameter<float> scale;
			ofParameter<int> ticks;
			ofParameter<bool> labels;
			ofParameter<bool> showX;
			ofParameter<bool> showY;
			ofParameter<bool> showZ;
		};
	}
}
