#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"
#include "ofxGrabScene.h"

#include "testNode.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);

	ofLight light;
	ofxGrabCam camera;
	ofxGrabScene scene; 
	
	testNode node1, node2;
};
