//
//  Handles.cpp
//  ofxGrabScene-example
//
#include "Handles.h"

namespace GrabScene {
	//---------
	Handles::Handles() :
	translateX(X_AXIS),
	translateY(Y_AXIS),
	translateZ(Z_AXIS),
	translateC(NO_AXIS),
	
	rotateX(X_AXIS),
	rotateY(Y_AXIS),
	rotateZ(Z_AXIS)
	
	{
	}
	
	//---------
	void Handles::init() {
		Translate::init();
		Rotate::init();
	}
}