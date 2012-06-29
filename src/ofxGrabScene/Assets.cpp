//
//  Assets.cpp
//  ofxGrabScene
//
#include "Assets.h"

namespace GrabScene {
	//---------
	Assets::Assets() {
		
	}
	
	//---------
	void Assets::init() {
		string filename, name, extension, withoutExtension;
		
		string dataPath = "ofxGrabScene";
		if (!ofDirectory::doesDirectoryExist(dataPath))
			dataPath = "../../../../../addons/ofxGrabScene/data/ofxCvGui";
		if (!ofDirectory::doesDirectoryExist(dataPath)) {
			ofLogError("ofxGrabScene") << "Assets data path cannot be found. Be sure to copy the ofxGrabScene folder from ofxGrabScene/data to your data folder!";
			return;
		}
		
		////
		//shaders
		////
		//
		string imageFolder = ofToDataPath(dataPath + "/shaders/", true);
		bool hasFrag, hasVert;
		ofDirectory files;
		files.listDir(imageFolder);
		for (int i=0; i<files.size(); i++) {
			filename = files.getPath(i);
			extension = ofFilePath::getFileExt(filename);
			withoutExtension = filename.substr(0, filename.length() - extension.length() - 1);
			name = ofFilePath::getBaseName(filename);
			
			if (!(extension == "vert" || extension == "frag"))
				continue;
			
			if (this->shaders.count(name) > 0)
				continue;
			
			hasFrag = ofFile::doesFileExist(withoutExtension + ".frag");
			hasVert = ofFile::doesFileExist(withoutExtension + ".vert");

			this->shaders.insert(pair<string, ofShader>(name, ofShader()));
			
			if (hasFrag && hasVert) {
				this->shaders[name].load(withoutExtension + ".frag", withoutExtension + ".vert");
			} else if (hasFrag || hasVert) {
				this->shaders[name].load(withoutExtension + ".frag");
			}
						
			ofLogNotice("ofxGrabScene") << "Loaded image asset '" << name << "'" << endl;
		}
		//
		////
	}

	//---------
	ofShader & Assets::getShader(string name) {
		if (this->shaders.count(name) != 0)
			return this->shaders[name];
		else
			return  this->blankShader;
	}

}