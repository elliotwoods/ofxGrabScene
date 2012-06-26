//
//  Node.h
//  ofxGrabScene
//
#pragma once

#include "ofNode.h"
#include "Handles.h"

namespace GrabScene {
	class Handles;
	
	class Node {
	public:
		Node(ofNode & node);
		void draw();
		ofNode * getNode() const;
		
		friend class Scene;		
	protected:
		ofNode * node;
		static Handles handles;
	};
}