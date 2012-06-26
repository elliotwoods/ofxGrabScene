//
//  Handles.cpp
//  ofxGrabScene-example
//
#include "Handles.h"

namespace GrabScene {
#pragma mark BaseHandle
	//---------
	Node * Handles::BaseHandle::parent = 0;
	
	//---------
	void Handles::BaseHandle::setParent(Node * node) {
		parent = node;
	}
	
#pragma mark Translate
	//---------
	ofMesh Handles::Translate::fill = ofMesh();
	ofMesh Handles::Translate::line = ofMesh();
	bool Handles::Translate::initialised = false;
	
	//---------
	Handles::Translate::Translate(Axis axis) {
		
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
		}
	}
	
	//---------
	void Handles::Translate::init() {
		if (Handles::Translate::initialised)
			return;
		
		Handles::Translate::initialised = true;
		
		const float radius = 0.05f;
		const float length = 1.0f;
		
		line.addVertex(ofVec3f(-radius,-radius, radius)); //0
		line.addVertex(ofVec3f(+radius,-radius, radius)); //1
		line.addVertex(ofVec3f(+radius,+radius, radius)); //2
		line.addVertex(ofVec3f(-radius,+radius, radius)); //3
		
		line.addVertex(ofVec3f(-radius,-radius, length)); //4
		line.addVertex(ofVec3f(+radius,-radius, length)); //5
		line.addVertex(ofVec3f(+radius,+radius, length)); //6
		line.addVertex(ofVec3f(-radius,+radius, length)); //7
		
		line.addVertex(ofVec3f(0.0f, 0.0f, length + radius)); //8
		
		//back
		line.addIndex(0);
		line.addIndex(1);
		line.addIndex(2);
		line.addIndex(3);
		line.addIndex(0);
		//left
		line.addIndex(4);
		line.addIndex(7);
		line.addIndex(3);
		line.addIndex(0);
		//bottom
		line.addIndex(4);
		line.addIndex(5);
		line.addIndex(1);
		//right
		line.addIndex(2);
		line.addIndex(6);
		line.addIndex(5);
		//tip
		line.addIndex(8);
		line.addIndex(6);
		line.addIndex(7);
		line.addIndex(8);
		line.addIndex(4);
		line.addIndex(5);
		
		line.setMode(OF_PRIMITIVE_LINE_STRIP);
		
		fill.addVertices(Handles::Translate::line.getVertices());
		
		//back
		fill.addIndex(0);
		fill.addIndex(2);
		fill.addIndex(1);
		
		fill.addIndex(0);
		fill.addIndex(3);
		fill.addIndex(2);
		
		//left
		fill.addIndex(0);
		fill.addIndex(4);
		fill.addIndex(7);
		
		fill.addIndex(0);
		fill.addIndex(7);
		fill.addIndex(3);
		
		//top
		fill.addIndex(7);
		fill.addIndex(2);
		fill.addIndex(3);
		
		fill.addIndex(7);
		fill.addIndex(6);
		fill.addIndex(2);
		
		//right
		fill.addIndex(1);
		fill.addIndex(6);
		fill.addIndex(5);
		
		fill.addIndex(1);
		fill.addIndex(2);
		fill.addIndex(6);		
		
		//bottom
		fill.addIndex(0);
		fill.addIndex(1);
		fill.addIndex(4);
		
		fill.addIndex(4);
		fill.addIndex(1);
		fill.addIndex(5);		
		
		//tip
		fill.addIndex(8);
		fill.addIndex(6);
		fill.addIndex(7);		
		
		fill.addIndex(4);
		fill.addIndex(8);
		fill.addIndex(7);
		
		fill.addIndex(4);
		fill.addIndex(5);
		fill.addIndex(8);
		
		fill.addIndex(8);
		fill.addIndex(5);
		fill.addIndex(6);
		
		fill.setMode(OF_PRIMITIVE_TRIANGLES);
	}

	//---------
	void Handles::Translate::draw() const {
		if (parent == 0)
			return;
		
		parent->getNode()->transformGL();
		ofPushMatrix();
		this->begin();
		
		ofPushStyle();
		ofSetLineWidth(0);
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(this->color, 100);
		fill.draw();
		
		ofSetLineWidth(3.0f);
		ofSetColor(this->color, 255);
		line.draw();
		
		ofPopStyle();
		
		ofPopMatrix();
		parent->getNode()->restoreTransformGL();
	}
	
	//---------
	void Handles::Translate::drawStencil() const {
		if (parent == 0)
			return;
		
		parent->getNode()->transformGL();
		ofPushMatrix();
		this->begin();
		fill.draw();
		ofPopMatrix();
		parent->getNode()->restoreTransformGL();
	}
	
	//---------
	void Handles::Translate::cursorDragged(const MovingCursor & cursor) {
		ofVec3f direction = this->getDirection();
		ofVec3f movement = cursor.worldViewFrameDifference.dot(direction) * direction;
		cout << "Movement: " << movement << endl;
		
		if (this->parent != 0)
			this->parent->getNode()->move(movement);
	}
	
	//---------
	ofVec3f Handles::Translate::getDirection() const {
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
		}
	}
	
	//---------
	void Handles::Translate::begin() const {
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
	
#pragma mark Handles
	//---------
	Handles::Handles() :
	translateX(X_AXIS), translateY(Y_AXIS), translateZ(Z_AXIS)
	{
	}
	
	//---------
	void Handles::init() {
		Translate::init();
	}
}