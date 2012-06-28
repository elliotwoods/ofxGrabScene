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
				this->fillColor.setHsb(0, GRABSCENE_HANDLES_SATURATION, GRABSCENE_HANDLES_FILL_BRIGHTNESS);
				this->lineColor.setHsb(0, 0, GRABSCENE_HANDLES_LINE_BRIGHTNESS);
				break;
				
			case Y_AXIS:
				this->fillColor.setHsb(255/3, GRABSCENE_HANDLES_SATURATION, GRABSCENE_HANDLES_FILL_BRIGHTNESS);
				this->lineColor.setHsb(255/3, 0, GRABSCENE_HANDLES_LINE_BRIGHTNESS);
				break;
				
			case Z_AXIS:
				this->fillColor.setHsb(255/3*2, GRABSCENE_HANDLES_SATURATION, GRABSCENE_HANDLES_FILL_BRIGHTNESS);
				this->lineColor.setHsb(255/3*2, 0, GRABSCENE_HANDLES_LINE_BRIGHTNESS);
				break;
				
			case NO_AXIS:
				this->fillColor = ofColor(200,200,200);
				this->lineColor = ofColor(255,255,255);
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
		ofSetColor(this->fillColor);
	}
	
	//---------
	void Handles::BaseHandle::setStyleLine() const {
		ofNoFill();
		ofSetLineWidth(2.0f);
		if (this->rollover)
			ofSetColor(255);
		else
			ofSetColor(this->lineColor);
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