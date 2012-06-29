//
//  Assets.h
//  ofxGrabScene
//
#pragma once

#include "ofShader.h"
#include <map>
#include <string>

namespace GrabScene {
	class Assets {
	public:
		Assets();
		void init();
		
		ofShader & getShader(string name);
		
	protected:
		map<string, ofShader> shaders;
		ofShader blankShader;
	};
	
	extern Assets AssetRegister;
}