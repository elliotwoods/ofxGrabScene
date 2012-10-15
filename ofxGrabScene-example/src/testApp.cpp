#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	
	for (int i=0; i<30; i++) {
		nodes.push_back(testNode());
		nodes.back().setPosition(ofRandom(-3.0f, +3.0f), ofRandom(-3.0f, +3.0f), ofRandom(-3.0f, +3.0f));
		nodes.back().rotate(ofRandom(180.0f), ofVec3f(ofRandom(1.0f), ofRandom(1.0f), ofRandom(1.0f)));
	}
	
	scene.init(camera);
	for (int i=0; i<nodes.size(); i++)
		scene.add(nodes[i]);
	
	scene.add(new GrabScene::Nodes::Grid);
	
	camera.setPosition(2.0f, 2.0f, 2.0f);
	camera.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
	
	light.setPointLight();
	light.setPosition(-3.0f, 3.0f, 3.0f);
	light.setAttenuation();
	
	ofFbo::Settings settings;
	settings.width = ofGetWidth();
	settings.height = ofGetHeight();
	settings.useDepth = true;
	settings.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
	settings.numSamples = 8;
	settings.internalformat = GL_RGB;
	fbo.allocate(settings);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	fbo.bind();
	ofClear(0);
    ofBackgroundGradient( ofColor(60), ofColor(0) );
	light.enable();
	fbo.unbind();
	
	//this is the single line you need to call.
	//it renders all the scene and the gui
	scene.draw(fbo);
	
	light.disable();
    
    ofDisableLighting();
	
	//fbo's should be drawn upside-down
	fbo.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'c')
		camera.toggleCursorDraw();
	if (key == ' ')
		GrabScene::AssetRegister.refresh();
}
