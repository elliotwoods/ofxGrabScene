//
//  Node.cpp
//  ofxGrabScene-example
//
#include "Node.h"

namespace GrabScene {
#pragma mark BaseNode
	//----------
	BaseNode::BaseNode() {
		this->translate.setName("Translate");
		this->rotate.setName("Rotate");
		
		this->translate.setMin(ofVec3f(-100.0f));
		this->translate.setMax(ofVec3f(-100.0f));
		
		this->parameters.add(this->translate);
		//this->parameters.add(this->rotate);
		this->translate.addListener(this, & BaseNode::translateCallback);
		this->rotate.addListener(this, & BaseNode::rotateCallback);
	}

	//----------
	string BaseNode::getName() const {
		return this->parameters.getName();
	}
	
	//----------
	void BaseNode::setName(const string & name) {
		return this->parameters.setName(name);
	}
	
	//----------
	void BaseNode::updateTranslate() {
		this->translate.setValue(this->getNode().getPosition());
	}
	
	//----------
	void BaseNode::updateRotate() {
		//this->rotate.setValue(this->getNode().getOrientationEuler());
		//cout << this->rotate << endl;
	}
	
	//----------
	void BaseNode::translateCallback(ofVec3f & value) {
		this->getNode().setPosition(value);
	}
	
	//----------
	void BaseNode::rotateCallback(ofVec3f & value) {
		//this->getNode().setOrientation(value);
	}
	
#pragma mark WrappedNode
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