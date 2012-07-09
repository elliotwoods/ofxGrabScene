//
//  Scene.cpp
//  ofxGrabScene
//

#include "Scene.h"

namespace GrabScene {
	//----------
	Scene::Scene() {
		this->elementUnderCursor = 0;
		this->nodeUnderCursor = false;
		this->nodeSelected = 0;
		this->initialised = false;
		this->lockIndex = false;
		
		this->elements.push_back(new NullElement());
		this->nodes.push_back(new NullNode());
		
		this->add(handles.translateX);
		this->add(handles.translateY);
		this->add(handles.translateZ);
		this->add(handles.translateC);
		this->add(handles.rotateX);
		this->add(handles.rotateY);
		this->add(handles.rotateZ);
	}
	
	//----------
	void Scene::init(ofxGrabCam & camera) {
		ofAddListener(ofEvents().update, this, &Scene::update);
		ofAddListener(ofEvents().mouseMoved, this, &Scene::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &Scene::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &Scene::mouseReleased);
		ofAddListener(ofEvents().mouseDragged, this, &Scene::mouseDragged);
		ofAddListener(ofEvents().keyPressed, this, &Scene::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &Scene::keyReleased);
		ofAddListener(AssetRegister.evtLoad, this, &Scene::assetsLoad);
		
		this->camera = &camera;
		this->initialised = true;
		
		AssetRegister.init();
		handles.init(camera);
	}
	
	//----------
	void Scene::draw() {
		if (!this->initialised) {
			ofSystemAlertDialog("You are calling ofxGrabScene::draw without first calling init, we're exiting!");
			ofExit();
		}
		
		ofPushStyle();
		ofEnableAlphaBlending();
		
		////
		//nodes
		node_iterator itN;
		for (itN = this->nodes.begin(); itN != this->nodes.end(); itN++) {
			(*itN)->draw();
		}
		//
		////
		
		
		////		
		//elements
		const_element_iterator itE;
		for (itE = elements.begin(); itE != elements.end(); itE++) {
			if (!(**itE).onTop())
				(**itE).draw();
		}
		//onTop
		if (frameBuffer.getWidth() != ofGetWidth() || frameBuffer.getHeight() != ofGetHeight()) {
			frameBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4);
		}
		//
		frameBuffer.bind();
		ofClear(0,0,0,0);
		for (itE = elements.begin(); itE != elements.end(); itE++) {
			if ((**itE).onTop())
				(**itE).draw();
		}
		frameBuffer.unbind();
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		shader("constant").begin();
		shader("constant").setUniformTexture("tex", frameBuffer, 0);
		drawFullscreen(frameBuffer);
		shader("constant").end();
		//
		////
		
		
		/////////////
		//indexBuffer
		/////////////
		//
		
		if (indexBuffer.getWidth() != ofGetWidth() || indexBuffer.getHeight() != ofGetHeight()) {
			indexBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
		}

		indexBuffer.bind();
		shader("index").begin();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		//draws an index at each pixel for elements and nodes underneath
		//first elements, then nodes
		int index = 0;
		
		for (itE = this->elements.begin(); itE != this->elements.end(); itE++) {
			if (!(*itE)->onTop()) {
				shader("index").setUniform1i("index", index);
				(*itE)->drawStencil();
			}
			index++;
		}

		for (itN = this->nodes.begin(); itN != this->nodes.end(); itN++) {
			shader("index").setUniform1i("index", index);
			(*itN)->drawStencil();
			index++;
		}

		ofVec4f reading;
		glReadPixels(ofGetMouseX(), ofGetHeight() - 1 - ofGetMouseY(), 1, 1, GL_RGBA, GL_FLOAT, &reading.x);
		const uint16_t reading1 = reading.x;

		//now redraw but with onTop elements
		glClear(GL_DEPTH_BUFFER_BIT);
		index = 0;

		for (itE = this->elements.begin(); itE != this->elements.end(); itE++) {
			if ((*itE)->onTop()) {
				shader("index").setUniform1i("index", index);
				(*itE)->drawStencil();
			}
			index++;
		}
		shader("index").end();

		if (!this->lockIndex) {
			glReadPixels(ofGetMouseX(), ofGetHeight() - 1 - ofGetMouseY(), 1, 1, GL_RGBA, GL_FLOAT, &reading);
			const uint16_t reading2 = reading.x;

			uint16_t returnedIndex = reading2 != 0 ? reading2 : reading1;

			//clear cut cases:
			if (returnedIndex == 0) {
				//we're not over anything
				this->elementUnderCursor = 0;
				this->nodeUnderCursor = 0;
			} else if (returnedIndex < this->elements.size()) {
				//we're over an element
				this->elementUnderCursor = returnedIndex;
				this->nodeUnderCursor = 0;
			} else if (returnedIndex - this->elements.size() < this->nodes.size()) {
				//we're over a node
				this->elementUnderCursor = 0;
				this->nodeUnderCursor = returnedIndex - this->elements.size();
			} else {
				//we're fucked
				ofLogError("GrabScene") << "Error when checking what's under the cursor";
			}
		}

		this->indexBuffer.unbind();
		//
		////


		////
		//draw outlines
		ofShader & outlineShader(shader("outlineIndex"));
		outlineShader.begin();
		outlineShader.setUniform1i("elementHover", this->elementUnderCursor);
		outlineShader.setUniform1i("nodeSelection", this->nodeSelected);
		outlineShader.setUniform1i("nodeHover", this->nodeUnderCursor);
		outlineShader.setUniformTexture("texIndex", indexBuffer, 2);
		outlineShader.setUniform1i("elementCount", this->elements.size());
		drawFullscreen(indexBuffer);
		outlineShader.end();
		//
		////
		
		ofPopStyle();
		

		////
		//cache values for unprojection of mouse
		this->viewport = ofGetCurrentViewport();
		glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix.getPtr());
		glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix.getPtr());
		glGetDoublev(GL_MODELVIEW_MATRIX, viewDoubles);
		glGetDoublev(GL_PROJECTION_MATRIX, projectionDoubles);
		//
		////
	}
	
	//----------
	void Scene::add(Element * element) {
		this->elements.push_back(element);
	}
	
	//----------
	void Scene::add(ofNode & node) {
		BaseNode * newNode = new WrappedNode(node);
		this->add(newNode);
	}
	
	//----------
	void Scene::add(BaseNode * const node) {
		this->nodes.push_back(node);
	}

	//----------
	bool Scene::hasSelection() const {
		return (this->nodeSelected > 0 && this->nodeSelected < this->nodes.size());
	}
	
	//----------
	BaseNode & Scene::getSelectedNode() {
		if (this->nodeSelected >= this->nodes.size()) {
			ofLogWarning("GrabScene") << "Selected node index is outside bounds of available nodes, selecting null node";
			this->nodeSelected = 0;
		}
		return *this->nodes[this->nodeSelected];
	}
	
	//----------
	void Scene::setSelectedNode(BaseNode & node) {
		const_node_iterator it;
		int index = 0;
		for (it = this->nodes.begin(); it != this->nodes.end(); it++) {
			if ((*it) == &node) {
				this->setSelectedNode(index);
				return;
			}
			index++;
		}
		
		ofLogError("GrabScene") << "node not found when calling setSelectedNode";
	}
	
	//----------
	void Scene::setSelectedNode(const uint16_t index) {
		if (index >= this->nodes.size()) {
			ofLogError("GrabScene") << "Node index " << index << " is outside the range of available nodes, selecting null node";
		} else {
			this->nodeSelected = index;
			Handles::BaseHandle::setParent(this->getSelectedNode());
			if (index == 0)
				Handles::BaseHandle::disable();
			else
				Handles::BaseHandle::enable();
			ofNotifyEvent(selectionChanged, this->nodeSelected, this);
		}
	}
	
	//----------
	Element & Scene::getElementUnderCursor() {
		if (this->elementUnderCursor >= this->elements.size()) {
			ofLogError("ofxGrabScene") << "Found index " << this->elementUnderCursor << " is out of range for this scene, which has " << this->elements.size() << " registered elements. Selecting null element.";
			this->elementUnderCursor = 0;
		}	
		return *this->elements[this->elementUnderCursor];
	}
	
	//----------
	ofFbo & Scene::getIndexBuffer() {
		return this->indexBuffer;
	}
	
	//----------
	const MovingCursor & Scene::getCursor() {
		return this->cursor;
	}
	
	//----------
	void Scene::drawFullscreen(ofBaseHasTexture & tex) {
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		//tex.getTextureReference().bind();
		
		const float width = tex.getTextureReference().getWidth();
		const float height = tex.getTextureReference().getHeight();
		
		ofMesh quad;
		quad.addVertex(ofVec3f(-1,-1,0));
		quad.addTexCoord(ofVec2f(0,0));
		quad.addVertex(ofVec3f(+1,-1,0));
		quad.addTexCoord(ofVec2f(width,0));
		quad.addVertex(ofVec3f(-1,+1,0));
		quad.addTexCoord(ofVec2f(0,height));
		quad.addVertex(ofVec3f(+1,+1,0));
		quad.addTexCoord(ofVec2f(width,height));
		
		quad.addIndex(0);
		quad.addIndex(1);
		quad.addIndex(3);
		
		quad.addIndex(0);
		quad.addIndex(3);
		quad.addIndex(2);
	
		quad.drawFaces();
		
		//tex.getTextureReference().unbind();
		
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
	
	//----------
	void Scene::updateCursor() {
		this->cursor.lastFrame = this->cursor;
		
		////
		//screen
		this->cursor.screen.x = ofGetMouseX();
		this->cursor.screen.y = ofGetMouseY();
		//
		////
		
		
		////
		//world
		float depth;
		
		//we presume we're using the stencil buffer which has 1,1 resolution
		//in order for this to be true, the update must be called when the fbo is attached
		glReadPixels(0, 0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		if (depth == 1.0f) {
			this->cursor.world = ofVec3f(0,0,0);
			
			this->cursor.worldValid = false;
		} else {
			GLdouble c[3];
			
			GLint viewportInt[4];
			viewportInt[0] = this->viewport.x;
			viewportInt[1] = ofGetWindowHeight() - this->viewport.y - this->viewport.height;
			viewportInt[2] = this->viewport.width;
			viewportInt[3] = this->viewport.height;
			
			gluUnProject(this->cursor.screen.x,
				ofGetHeight()-1-this->cursor.screen.y,
				depth, this->viewDoubles, this->projectionDoubles,
				viewportInt, c, c+1, c+2);
			
			this->cursor.world.x = c[0];
			this->cursor.world.y = c[1];
			this->cursor.world.z = c[2];
			
			this->cursor.worldValid = true;
		}
		//
		////
		
		
		////
		//worldViewFrameDifference
		ofVec3f screenDiffNorm = this->cursor.getScreenFrameDifference() / ofVec2f(viewport.width, -viewport.height);
		this->cursor.worldViewFrameDifference = screenDiffNorm * viewMatrix.getInverse().getRotate();
		float distance = (this->camera->getPosition() - this->cursor.world).length();
		this->cursor.worldViewFrameDifference *= distance * tan(this->camera->getFov() * DEG_TO_RAD / 2.0f) * 2.0f * 2.0f;
		this->cursor.worldViewFrameDifference.x *= ofGetWidth() / ofGetHeight();
		//
		///

		this->cursor.lastUpdate = ofGetFrameNum();
	}
	
	//----------
	void Scene::update(ofEventArgs & args) {
		if (this->hasSelection()) {
			float distanceToNode = (this->camera->getPosition() - this->getSelectedNode().getNode().getPosition()).length();
			float planeHeight = tan(camera->getFov() * DEG_TO_RAD / 2.0f) * 2.0f * distanceToNode;
			Handles::BaseHandle::setScale(planeHeight / 8.0f);
		}
	}
	
	
	//----------
	void Scene::mouseMoved(ofMouseEventArgs & args) {
		this->updateCursor();
		this->getElementUnderCursor();
		
		//help ofxGrabCam by using our version of the world cursor
//		if (this->cursor.world != ofVec3f(0,0,0))
//			this->camera->setCursorWorld(this->cursor.world);
	}
	
	//----------
	void Scene::mousePressed(ofMouseEventArgs & args) {
		this->updateCursor();
		this->cursor.start(args.button);
		this->cursor.captured = args.button == 0 && !ofGetKeyPressed();
		this->cursor.dragged = false;
		
		this->getElementUnderCursor().cursorDown(cursor);
		
		if (cursor.captured) {
			this->camera->setMouseActions(false);
		}
		this->lockIndex = true;
	}
	
	//----------
	void Scene::mouseReleased(ofMouseEventArgs & args) {
		this->updateCursor();
		this->cursor.end(args.button);
		this->getElementUnderCursor().cursorReleased(this->cursor);
		this->camera->setMouseActions(true);
		this->lockIndex = false;
		
		//click
		if (!this->cursor.dragged && this->elementUnderCursor==0) {
			this->setSelectedNode(nodeUnderCursor);
		}
	}
	
	//----------
	void Scene::mouseDragged(ofMouseEventArgs & args) {
		if (this->cursor.captured) {
			this->updateCursor();
			this->getElementUnderCursor().cursorDragged(this->cursor);
		}
		this->cursor.dragged = true;
	}
	
	
	//----------
	void Scene::keyPressed(ofKeyEventArgs & args) {
		
	}
	
	//----------
	void Scene::keyReleased(ofKeyEventArgs & args) {
		
	}
	
	//----------
	void Scene::assetsLoad(GrabScene::Assets &assets) {
		
	}
}