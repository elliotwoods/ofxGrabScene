//
//  Assets.h
//  ofxGrabScene
//
#pragma once

#include "ofShader.h"
#include "ofImage.h"
#include "ofEvents.h"
#include <map>
#include <string>

namespace GrabScene {
	class Assets {
	public:
		Assets();
		void init();
		void refresh();
		
		ofImage & getImage(string name);
		ofShader & getShader(string name);
		
		ofEvent<Assets> evtLoad;
	protected:
		
		map<string, ofImage> images;
		map<string, ofShader> shaders;
		
		ofImage blankImage;
		ofShader blankShader;

	};
	
	extern Assets AssetRegister;
	
	static ofShader & shader(string name) { return AssetRegister.getShader(name); }
	static ofImage & image(string name) { return AssetRegister.getImage(name); }
}