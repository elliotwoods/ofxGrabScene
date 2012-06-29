#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	
	scene.init(camera);
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
	ofBackground(40);
	camera.begin();
	light.enable();
	
	ofDrawGrid(10.0f);
	scene.draw();
	
	light.disable();
	ofDisableLighting();
	camera.end();
	
//	scene.getIndexBuffer().draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'c')
		camera.toggleCursorDraw();
}
