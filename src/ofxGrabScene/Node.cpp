//
//  Node.cpp
//  ofxGrabScene-example
//
#include "Node.h"

namespace GrabScene {
	//----------
	WrappedNode::WrappedNode(ofNode & node) {
		this->node = &node;
	}
	
	//----------
	void WrappedNode::draw() {
		this->node->draw();
	}
	
	//----------
	void WrappedNode::drawStencil() {
		this->node->draw();
	}
	
	//----------
	ofNode & WrappedNode::getNode() {
		return *this->node;
	}
}