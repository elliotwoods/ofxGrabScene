//
//  BaseHandle.cpp
//  ofxGrabScene
//
#include "Handles.h"

namespace GrabScene {
	//---------
	Node * Handles::BaseHandle::parent = 0;
	
	//---------
	Handles::BaseHandle::BaseHandle(Axis axis) {
		this->rollover = false;
		this->axis = axis;
		
		switch (this->axis) {
			case X_AXIS:
				color = ofColor(255,100,100);
				break;
				
			case Y_AXIS:
				color = ofColor(100,255,100);
				break;
				
			case Z_AXIS:
				this->color = ofColor(100,100,255);
				break;
				
			case NO_AXIS:
				color = ofColor(255,255,255);
				break;
		}
	}
	
	//---------
	void Handles::BaseHandle::setParent(Node * node) {
		parent = node;
	}
	
	//---------
	ofVec3f Handles::BaseHandle::getDirection() const {
		switch(axis) {
			case X_AXIS:
				return ofVec3f(1, 0, 0);
				break;
			case Y_AXIS:
				return ofVec3f(0, 1, 0);
				break;
			case Z_AXIS:
				return ofVec3f(0, 0, 1);
				break;
			case NO_AXIS:
				return ofVec3f(1, 1, 1);
				break;
		}
	}
	
	//---------
	void Handles::BaseHandle::rotateAxis() const {
		switch (this->axis) {
			case X_AXIS:
				ofRotate(90, 0, 1, 0);
				break;
				
			case Y_AXIS:
				ofRotate(-90, 1, 0, 0);
				break;
				
			default:
				break;
		}
	}
	
	//---------
	void Handles::BaseHandle::setStyleFill() const {
		ofFill();
		ofSetLineWidth(0.0f);
		ofSetColor(this->color, 150);//this->rollover ? 255 : 150);
	}
	
	//---------
	void Handles::BaseHandle::setStyleLine() const {
		ofNoFill();
		ofSetLineWidth(2.0f);
		ofSetColor(this->color, this->rollover ? 255 : 0);
	}

	//---------
	void Handles::BaseHandle::cursorOver(const Cursor &) {
		this->rollover = true;
	}
	
	//---------
	void Handles::BaseHandle::cursorOut(const Cursor &) {
		this->rollover = false;		
	}
	
}