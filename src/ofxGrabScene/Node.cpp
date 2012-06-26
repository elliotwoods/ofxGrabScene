//
//  Node.cpp
//  ofxGrabScene-example
//
#include "Node.h"

namespace GrabScene {
	//----------
	Handles Node::handles = Handles();
	
	//----------
	Node::Node(ofNode & node) {
		this->node = &node;
	}
	
	//----------
	void Node::draw() {
		this->node->draw();
	}
	
	//----------
	ofNode * Node::getNode() const {
		return this->node;
	}
}