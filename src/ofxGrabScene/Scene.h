//
//  Scene.h
//  ofxGrabScene
//

#pragma once

#include "ofMain.h"

namespace ofxGrabScene{
	class Scene : public ofNode {
	public:
		Scene();
		void init();
		void draw();
		
	protected:
		
		////
		//events
		//
		void	update(ofEventArgs & args);
		void    mouseMoved(ofMouseEventArgs & args);
		void	mousePressed(ofMouseEventArgs & args);
		void	mouseReleased(ofMouseEventArgs & args);
		void	mouseDragged(ofMouseEventArgs & args);
		void	keyPressed(ofKeyEventArgs & args);
		void	keyReleased(ofKeyEventArgs & args);
		//
		bool	initialised;
		bool	mouseActions;
		//
		////
	};
}