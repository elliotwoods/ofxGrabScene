#pragma once

#include "ofMain.h"

#include "ofxGrabCam.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);

	ofxGrabCam camera;
};
