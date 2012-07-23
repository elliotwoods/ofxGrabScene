//
//  Rotate.cpp
//  ofxGrabScene
//
#include "../Handles.h"

namespace GrabScene {
	//---------
	ofMesh Handles::Rotate::fill = ofMesh();
	
	//---------
	Handles::Rotate::Rotate(Axis axis) : BaseHandle(axis) {

	}
	
	//---------
	void Handles::Rotate::init() {
		const float radius = GRABSCENE_HANDLES_RADIUS_1;
		const float width = GRABSCENE_HANDLES_RADIUS_2;
		
		const int resolution = 48.0f;
		
		float theta;
		
		////
		//vertices
		
		//inner ring
		for (int i=0; i<resolution; i++) {
			theta = (float) i / (float) resolution * TWO_PI;
			fill.addVertex(ofVec3f(radius * cos(theta), radius * sin(theta), 0.0f));
		}
						   
		//outer ring
		for (int i=0; i<resolution; i++) {
			theta = ((float) i + 0.5f) / (float) resolution * TWO_PI;
			fill.addVertex(ofVec3f((radius + width) * cos(theta), (radius + width) * sin(theta), 0.0f));
		}
		//
		////
		
		
		////
		//indices
		for (int i=0; i<resolution+1; i++) {
			fill.addIndex(i % resolution);
			fill.addIndex((i % resolution) + resolution);
		}
		//
		////
		
		////
		//styles
		fill.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		//
		////
	}
	
	//---------
	void Handles::Rotate::draw() const {
		if (parent == 0 || !this->enabled)
			return;
		
		GLboolean hadLighting;
		glGetBooleanv(GL_LIGHTING, &hadLighting);
		if (hadLighting)
			ofDisableLighting();
		
		ofPushMatrix();
		this->doTransform();
		
		ofPushStyle();
		shader("fixed").begin();
		
		this->rotateAxis();
		this->setStyleFill();
		fill.draw();
		
		shader("fixed").end();
		
		ofSetColor(255);
		if (this->rollover) {
			ofTranslate(0, GRABSCENE_HANDLES_RADIUS_1 + GRABSCENE_HANDLES_RADIUS_2 * 2);
			ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
			ofDrawBitmapString(this->getReading(), ofPoint());
		}
		
		ofPopStyle();
		
		ofPopMatrix();
		
		if (hadLighting)
			ofEnableLighting();
	}
	
	//---------
	void Handles::Rotate::drawStencil() const {
		if (parent == 0 || !this->enabled)
			return;
		
		ofPushMatrix();
		this->doTransform();
		
		this->rotateAxis();
		fill.draw();
		
		ofPopMatrix();
	}
	
	//---------
	void Handles::Rotate::cursorDragged(const MovingCursor & cursor) {
		if (this->parent == 0) {
			ofLogError("GrabScene") << "No parent node set for handle";
			return;
		}
		
		ofNode & node(this->parent->getNode());
		ofVec3f center = node.getPosition();
		ofVec3f direction = this->getDirection() * node.getOrientationQuat();
		
		//find element of vector perpendicular to direction
		ofVec3f perp = cursor.worldViewFrameDifference;
		perp -= perp.dot(direction) * direction;
		
		ofVec3f v1, v2;
		v1 = (cursor.startWorld - center).normalized();
		v2 = (cursor.startWorld + perp - center).normalized();
		ofQuaternion rotation;
		rotation.makeRotate(v1,v2);
		
		float angle;
		ofVec3f direction2;
		rotation.getRotate(angle, direction2);
		if (direction2.dot(direction) < 0.0f)
			angle *= -1.0f;
		
		//check valid
		if (angle == angle && direction == direction) {
			node.rotate(angle, direction);
			this->parent->updateRotate();
		}
	}
	
	//---------
	void Handles::Rotate::doTransform() const {
		ofTranslate(parent->getNode().getPosition());
		glMultMatrixf(ofMatrix4x4(parent->getNode().getOrientationQuat()).getPtr());
		ofScale(scale, scale, scale);
	}
	
	//---------
	string Handles::Rotate::getReading() const {
		if (this->parent == 0)
			return "";
		
		ofNode & node(this->parent->getNode());
		
		return ofToString(node.getOrientationEuler().dot(this->getDirection()), 2);
	}
}