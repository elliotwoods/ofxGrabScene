//
//  Translate.cpp
//  ofxGrabScene
//
#include "../Handles.h"

namespace GrabScene {
	//---------
	ofMesh Handles::Translate::fill = ofMesh();
	
	//---------
	Handles::Translate::Translate(Axis axis) :
	BaseHandle(axis) {
	
	}
	
	//---------
	void Handles::Translate::init() {		
		const float length = GRABSCENE_HANDLES_RADIUS_1;
		const float radius = GRABSCENE_HANDLES_RADIUS_2;
		const float width = GRABSCENE_HANDLES_RADIUS_2 * 2.0f / 3.0f;
		
		fill.addVertex(ofVec3f(-width,-width, radius * 2)); //0
		fill.addVertex(ofVec3f(+width,-width, radius * 2)); //1
		fill.addVertex(ofVec3f(+width,+width, radius * 2)); //2
		fill.addVertex(ofVec3f(-width,+width, radius * 2)); //3
		
		fill.addVertex(ofVec3f(-width,-width, length - radius)); //4
		fill.addVertex(ofVec3f(+width,-width, length - radius)); //5
		fill.addVertex(ofVec3f(+width,+width, length - radius)); //6
		fill.addVertex(ofVec3f(-width,+width, length - radius)); //7
		
		fill.addVertex(ofVec3f(0.0f, 0.0f, length)); //8
		
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
		if (parent == 0 || !this->enabled)
			return;
		
		GLboolean hadLighting;
		glGetBooleanv(GL_LIGHTING, &hadLighting);
		if (hadLighting)
			ofDisableLighting();
		
		ofPushMatrix();
		ofTranslate(parent->getNode().getPosition());
		ofTranslate(parent->getNode().getOrientationQuat() * parent->getOrigin());
		ofScale(scale, scale, scale);
								 
		ofPushStyle();
		if (this->axis == NO_AXIS) {
			this->setStyleFill();
			ofBox(GRABSCENE_HANDLES_RADIUS_2 * 2);
		} else {
			this->rotateAxis();
			this->setStyleFill();
			fill.draw();
		}
		ofPopStyle();
		
		if (this->rollover) {
			ofTranslate(0, 0, GRABSCENE_HANDLES_RADIUS_1 + GRABSCENE_HANDLES_RADIUS_2 * 2);
			ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
			ofDrawBitmapString(this->getReading(), ofPoint());
		}
		
		ofPopMatrix();
		
		if (hadLighting)
			ofEnableLighting();
	}
	
	//---------
	void Handles::Translate::drawStencil() const {
		if (parent == 0 || !this->enabled)
			return;
		
		ofPushMatrix();
		ofTranslate(parent->getNode().getPosition());
		ofTranslate(parent->getNode().getOrientationQuat() * parent->getOrigin());
		ofScale(scale, scale, scale);
		
		if (this->axis == NO_AXIS) {
			ofBox(GRABSCENE_HANDLES_RADIUS_2 * 2);
		} else {
			this->rotateAxis();
			fill.draw();
			
		}
		
		ofPopMatrix();
	}
	
	//---------
	void Handles::Translate::cursorDragged(const MovingCursor & cursor) {
		if (this->parent == 0) {
			ofLogError("GrabScene") << "No parent node set for handle";
			return;
		}
		if (this->camera == 0) {
			ofLogError("GrabScene") << "No camera set for handle";
			return;
		}
		
		ofVec3f direction = this->getDirection();
		ofVec3f movement;
		if (this->axis == NO_AXIS) {
			movement = cursor.worldViewFrameDifference / 2;
		} else {
			// s = start
			// t = ray vector
			// view = in camera view
			const ofVec3f s = this->parent->getNode().getPosition();
			const ofVec3f t = direction;
			
			ofVec3f sView = this->camera->worldToScreen(s);
			ofVec3f tView = this->camera->worldToScreen(s + t) - sView;
			ofVec3f dXY = cursor.getScreenFrameDifference();
			
			sView.z = 0.0f;
			tView.z = 0.0f;
			
			movement = t * tView.dot(dXY) / (tView.length() * tView.length());
		}
		
		//check valid
		if (movement.lengthSquared() == movement.lengthSquared()) {
			this->parent->getNode().move(movement);
			this->parent->updateTranslate();
		}
	}
	
	//---------
	string Handles::Translate::getReading() const {
		if (this->parent == 0)
			return "";
		
		ofNode & node(this->parent->getNode());
		
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