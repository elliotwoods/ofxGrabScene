#include "testApp.h"

class testElement : public ofxGrabSceneElement {
public:
	//---------
	testElement() {
		this->highlighted = false;
	}
	
	//---------
	void draw() const {
		ofPushMatrix();
		ofTranslate(position);
		
		ofPushStyle();
		ofEnableAlphaBlending();
		
		ofNoFill();
		ofSetColor(255.0f, 0.0f, 0.0f);
		ofSetLineWidth(2.0f);
		ofBox(0.3f);
		
		ofFill();
		ofSetLineWidth(0.0f);
		if (this->highlighted)
			ofSetColor(255, 0, 0, 100);
		else
			ofSetColor(255, 0, 0, 50);
		ofBox(0.3f);
		
		ofPopStyle();
		
		ofPopMatrix();
	}
	
	//---------
	void drawStencil() const {
		ofPushMatrix();
		ofTranslate(position);
		ofFill();
		ofBox(0.3f);
		ofPopMatrix();
	}
	
	//---------
	void cursorOver(const GrabScene::Cursor &) { 
		this->highlighted = true;
	}
	
	//---------	
	void cursorOut(const GrabScene::Cursor &) {
		this->highlighted = false;
	}
	
	//---------
	void cursorDragged(const GrabScene::MovingCursor & cursor) {
		if (cursor.button[0]) {
			this->position += cursor.getWorldFrameDifference();
		}
	}
	
	bool highlighted;
	ofVec3f position;
};

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	
	scene.init(camera);
	//scene.add(new testElement());
	scene.add(node);
	
	camera.setPosition(2.0f, 2.0f, 2.0f);
	camera.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
	
	light.setPointLight();
	light.setPosition(-3.0f, 3.0f, 3.0f);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(50);
	camera.begin();
	light.enable();
	
	ofDrawGrid(10.0f);
	//scene.getCursor().draw();
	scene.draw();
	
	light.disable();
	ofDisableLighting();
	camera.end();
	
	glColor3f(100.0f, 100.0f, 100.0f);
	scene.getIndexBuffer().draw(0,0);
	glColor3f(1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'c')
		camera.toggleCursorDraw();
}
