//
//  testNode.h
//  ofxGrabScene-example
//

#pragma once
#include "ofMain.h"

class testNode : public ofNode {
public:
	void customDraw() {
		ofSphere(0.5f);
	}
};