//
//  testNode.h
//  ofxGrabScene-example
//

#pragma once
#include "ofMain.h"

class testNode : public ofNode {
public:
	void customDraw() {
		ofPushStyle();
		ofSetColor(150,150,150);
		ofBox(0.7f);
		ofPopStyle();
	}
};