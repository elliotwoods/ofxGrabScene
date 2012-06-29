//
//  Translate.cpp
//  ofxGrabScene
//
#include "../Handles.h"

namespace GrabScene {
	//---------
	ofMesh Handles::Translate::fill = ofMesh();
	ofMesh Handles::Translate::line = ofMesh();
	
	//---------
	Handles::Translate::Translate(Axis axis) :
	BaseHandle(axis) {
	
	}
	
	//---------
	void Handles::Translate::init() {		
		const float length = GRABSCENE_HANDLES_RADIUS_1;
		const float radius = GRABSCENE_HANDLES_RADIUS_2;
		const float width = GRABSCENE_HANDLES_RADIUS_2 * 2.0f / 3.0f;
		
		line.addVertex(ofVec3f(-width,-width, radius * 2)); //0
		line.addVertex(ofVec3f(+width,-width, radius * 2)); //1
		line.addVertex(ofVec3f(+width,+width, radius * 2)); //2
		line.addVertex(ofVec3f(-width,+width, radius * 2)); //3
		
		line.addVertex(ofVec3f(-width,-width, length - radius)); //4
		line.addVertex(ofVec3f(+width,-width, length - radius)); //5
		line.addVertex(ofVec3f(+width,+width, length - radius)); //6
		line.addVertex(ofVec3f(-width,+width, length - radius)); //7
		
		line.addVertex(ofVec3f(0.0f, 0.0f, length)); //8
		
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
		
		ofPushMatrix();
		ofTranslate(parent->getNode()->getPosition());
		ofScale(scale, scale, scale);
		
		ofPushStyle();
		if (this->axis == NO_AXIS) {
			this->setStyleFill();
			ofBox(GRABSCENE_HANDLES_RADIUS_2 * 2);
			
			this->setStyleLine();
			ofBox(GRABSCENE_HANDLES_RADIUS_2 * 2);
		} else {
			this->rotateAxis();
			this->setStyleFill();
			fill.draw();
			
			this->setStyleLine();
			line.draw();
			
		} 
		ofPopStyle();
		
		if (this->rollover) {
			ofTranslate(0, 0, GRABSCENE_HANDLES_RADIUS_1 + GRABSCENE_HANDLES_RADIUS_2 * 2);
			ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
			ofDrawBitmapString(this->getReading(), ofPoint());
		}
		
		ofPopMatrix();
	}
	
	//---------
	void Handles::Translate::drawStencil() const {
		if (parent == 0)
			return;
		
		ofPushMatrix();
		ofTranslate(parent->getNode()->getPosition());
		ofScale(scale, scale, scale);
		
		if (this->axis == NO_AXIS) {
			ofBox(GRABSCENE_HANDLES_RADIUS_2);
		} else {
			this->rotateAxis();
			fill.draw();
			
		}
		
		ofPopMatrix();
	}
	
	//---------
	void Handles::Translate::cursorDragged(const MovingCursor & cursor) {
		ofVec3f direction = this->getDirection();
		ofVec3f movement;
		if (this->axis == NO_AXIS) {
			movement = cursor.worldViewFrameDifference;
		} else {
			movement = cursor.worldViewFrameDifference.dot(direction) * direction;
		}
		
		cout << "Movement: " << movement << endl;
		
		if (this->parent != 0)
			this->parent->getNode()->move(movement);
	}
	
	//---------
	string Handles::Translate::getReading() const {
		if (this->parent == 0)
			return "";
		
		ofNode & node(*this->parent->getNode());
		
		if (this->axis == NO_AXIS) {
			string reading;
			ofVec3f position = node.getPosition();
			reading += ofToString(position.x, 2) + ", ";
			reading += ofToString(position.y, 2) + ", ";
			reading += ofToString(position.z, 2);
			return reading;
		} else {
			return ofToString(node.getPosition().dot(this->getDirection()), 2);
		}
	}
}