//
//  Panel.h
//  ofxGrabScene
//
#pragma once

#include "ofxGui.h"

#include "../Node.h"

namespace GrabScene {
	class Panel : public ofxPanel {
	public:
		Panel();
		
		void setTarget(BaseNode &);
	};
}