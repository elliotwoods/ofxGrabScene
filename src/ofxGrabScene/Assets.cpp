//
//  Assets.cpp
//  ofxGrabScene
//
#include "Assets.h"

namespace GrabScene {
	//---------
	Assets AssetRegister = Assets();
	
	//---------
	Assets::Assets() {
		
	}
	
	//---------
	void Assets::init() {
		ofLogNotice("ofxGrabScene") << "//--------------------";
		ofLogNotice("ofxGrabScene") << "//Assets::init";
		ofLogNotice("ofxGrabScene") << "//--------------------";
		ofLogNotice("ofxGrabScene") << "//";
		
		string filename, name, extension, withoutExtension, folder;
		ofDirectory files;
		
		string dataPath = "ofxGrabScene";
		if (!ofDirectory::doesDirectoryExist(dataPath))
			dataPath = "../../../../../addons/ofxGrabScene/data/ofxCvGui";
		if (!ofDirectory::doesDirectoryExist(dataPath)) {
			ofLogError("ofxGrabScene") << "Assets data path cannot be found. Be sure to copy the ofxGrabScene folder from ofxGrabScene/data to your data folder!";
			return;
		}
		
		////
		//images
		////
		//
		folder = ofToDataPath(dataPath + "/images/", true);
		files.listDir(folder);
		for (int i=0; i<files.size(); i++) {
			filename = files.getPath(i);
			extension = ofFilePath::getFileExt(filename);
			withoutExtension = filename.substr(0, filename.length() - extension.length() - 1);
			name = ofFilePath::getBaseName(filename);
			
			if (!(extension == "png" || extension == "jpeg" || extension == "jpg"))
				continue;
			
			if (this->images.count(name) > 0)
				continue;
			
			this->images.insert(pair<string, ofImage>(name, ofImage()));
			
			this->images[name].loadImage(filename);
			
			ofLogNotice("ofxGrabScene") << "Loaded image asset '" << name << "'" << endl;
		}
		//
		////
		
		
		////
		//shaders
		////
		//
		folder = ofToDataPath(dataPath + "/shaders/", true);
		bool hasFrag, hasVert;
		files.listDir(folder);
		for (int i=0; i<files.size(); i++) {
			filename = files.getPath(i);
			extension = ofFilePath::getFileExt(filename);
			withoutExtension = filename.substr(0, filename.length() - extension.length() - 1);
			name = ofFilePath::getBaseName(filename);
			
			if (!(extension == "vert" || extension == "frag" || extension == "geom"))
				continue;
			
			if (this->shaders.count(name) > 0)
				continue;
			
			this->shaders.insert(pair<string, ofShader>(name, ofShader()));
			
			if (ofFile::doesFileExist(withoutExtension + ".frag"))
				this->shaders[name].setupShaderFromFile(GL_FRAGMENT_SHADER, withoutExtension + ".frag");
			if (ofFile::doesFileExist(withoutExtension + ".vert"))
				this->shaders[name].setupShaderFromFile(GL_VERTEX_SHADER, withoutExtension + ".vert");
			if (ofFile::doesFileExist(withoutExtension + ".geom"))
				this->shaders[name].setupShaderFromFile(GL_GEOMETRY_SHADER, withoutExtension + ".geom");
			this->shaders[name].linkProgram();
							
			ofLogNotice("ofxGrabScene") << "Loaded shader asset '" << name << "'" << endl;
		}
		//
		////
		
		ofLogNotice("ofxGrabScene") << "//";
		ofLogNotice("ofxGrabScene") << "//--------------------";
		
		ofNotifyEvent(evtLoad, *this, this);
	}
	
	//---------
	void Assets::refresh() {
		this->shaders.clear();
		this->images.clear();
		this->init();
	}

	//---------
	ofShader & Assets::getShader(string name) {
		if (this->shaders.count(name) != 0)
			return this->shaders[name];
		else {
			ofLogError("GrabScene") << "Requested shader asset'" << name << "' doesn't exist, have you got all the files in the right place in your data/ folder?";
			return  this->blankShader;
		}
	}
	
	//---------
	ofImage & Assets::getImage(string name) {
		if (this->images.count(name) != 0)
			return this->images[name];
		else {
			ofLogError("GrabScene") << "Requested image asset'" << name << "' doesn't exist, have you got all the files in the right place in your data/ folder?";
			return  this->blankImage;
		}
	}

}