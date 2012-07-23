//
//  Panel.cpp
//  ofxGrabScene
//
#include "Panel.h"

namespace GrabScene {
	//---------
	Panel::Panel() {

	}
	
	//---------
	void Panel::setTarget(BaseNode & node) {
		this->clear();
		this->add(node.parameters);
	}
}