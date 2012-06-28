//
//  Scene.cpp
//  ofxGrabScene
//

#include "Scene.h"

namespace GrabScene {
	//----------
	Scene::Scene() {
		this->indexCachedFrame = -1;
		this->index = 0;
		this->lockIndex = false;
		this->selection = 0;
		this->elements.push_back(new NullElement());
		indexBuffer.allocate(256, 256, GL_RGB32F);
		
		Node::handles.init();
		this->add(Node::handles.translateX);
		this->add(Node::handles.translateY);
		this->add(Node::handles.translateZ);
		this->add(Node::handles.translateC);
		this->add(Node::handles.rotateX);
		this->add(Node::handles.rotateY);
		this->add(Node::handles.rotateZ);
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
		
		this->camera = &camera;
	}
	
	//----------
	void Scene::draw() {
		node_iterator itN;
		for (itN = this->nodes.begin(); itN != this->nodes.end(); itN++) {
			(*itN)->node->draw();
		}
		
		const_element_iterator it;
		//standard
		for (it = elements.begin(); it != elements.end(); it++) {
			if (!(**it).onTop())
				(**it).draw();
		}
		//onTop
		startOnTop();
		for (it = elements.begin(); it != elements.end(); it++) {
			if ((**it).onTop())
				(**it).draw();
		}
		endOnTop();
		
		this->viewport = ofGetCurrentViewport();
		glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix.getPtr());
		glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix.getPtr());
		glGetDoublev(GL_MODELVIEW_MATRIX, viewDoubles);
		glGetDoublev(GL_PROJECTION_MATRIX, projectionDoubles);
	}
	
	//----------
	void Scene::add(Element * element) {
		this->elements.push_back(element);
	}
	
	//----------
	void Scene::add(ofNode & node) {
		this->nodes.push_back(new Node(node));
		this->setSelectedNode(*nodes.back());
	}
	
	//----------
	Node * const Scene::getSelectedNode() const {
		return this->selection;
	}
	
	//----------
	void Scene::setSelectedNode(Node & node) {
		this->selection = &node;
		Handles::BaseHandle::setParent(&node);
		return this->selection;
	}
	
	
	//----------
	Element * const Scene::getElementUnderCursor() {
		if (this->index < this->elements.size()) {
			return this->elements[this->index];
		} else {
			ofLogError("ofxGrabScene") << "Found index " << this->index << " is out of range for this scene, which has " << this->elements.size() << " registered elements.";
			return this->elements[0]; //null element
		}
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
	void Scene::startOnTop() const {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		ofMatrix4x4 projectionScaled;
		glGetFloatv(GL_PROJECTION_MATRIX, projectionScaled.getPtr());
		projectionScaled.postMultScale(ofVec3f(1.0f, 1.0f, GRABSCENE_ON_TOP_SCALE));
		//glLoadMatrixf(projectionScaled.getPtr());
		glMatrixMode(GL_MODELVIEW);
	}
	
	//----------
	void Scene::endOnTop() const {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	
	//----------
	void Scene::updateCursorAndIndex() {
		if (indexCachedFrame >= ofGetFrameNum())
			return;
		
		indexBuffer.begin();
		glEnable(GL_DEPTH_TEST);
		ofClear(0.0f);
		
		ofPushMatrix();
		//
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		ofScale(+1.0f, -1.0f);
		ofVec2f centerCursor;
		centerCursor.x = (ofGetMouseX() - this->viewport.x) / (this->viewport.width / 2.0f);
		centerCursor.y = (-ofGetMouseY() - this->viewport.y) / (this->viewport.height / 2.0f);
		ofTranslate(-centerCursor.x, -centerCursor.y);
		glMultMatrixf(projectionMatrix.getPtr());
		//
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(viewMatrix.getPtr());
		
		ofPushStyle();
		ofBackground(0);
		ofDisableSmoothing();
		ofDisableAlphaBlending();
		
		float scaledValue;
		//standard
		for (int i=0; i<elements.size(); i++) {
			if (elements[i]->onTop())
				continue;
			scaledValue = float(i) / float(1 << 10);
			glColor3f(scaledValue, scaledValue, scaledValue);
			elements[i]->drawStencil();
		}
		//onTop
		startOnTop();
		for (int i=0; i<elements.size(); i++) {
			if (!elements[i]->onTop())
				continue;
			scaledValue = float(i) / float(1 << 10);
			glColor3f(scaledValue, scaledValue, scaledValue);
			elements[i]->drawStencil();
		}
		endOnTop();
		
		ofPopStyle();
		
		ofPopMatrix();
		
		updateCursor();
		glDisable(GL_DEPTH_TEST);
		
		if (!this->lockIndex) {
			float rawValue;
			glReadPixels(0, 0, 1, 1, GL_RED, GL_FLOAT, &rawValue);
			unsigned short readIndex = rawValue * float(1 << 10) + 0.5f;
			indexCachedFrame = ofGetFrameNum();
			
			//perform cursor over, cursor out actions
			if (this->index != readIndex) {
				if (this->index != 0)
					this->getElementUnderCursor()->cursorOut(cursor);
				
				this->index = readIndex;
				cout << "Found index " << this->index << endl;
				
				if (this->index != 0)
					this->getElementUnderCursor()->cursorOver(cursor);
			}
		}
		indexBuffer.end();
	}
	
	//----------
	void Scene::updateCursor() {
		
		//DO NOT CALL THIS FUNCTION DIRECTLY
		
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
		this->cursor.worldViewFrameDifference *= distance * tan(this->camera->getFov() * DEG_TO_RAD / 2.0f) * 2.0f;
		//
		///

		this->cursor.lastUpdate = ofGetFrameNum();
	}
	
	//----------
	void Scene::update(ofEventArgs & args) {
		this->updateCursorAndIndex();
	}
	
	
	//----------
	void Scene::mouseMoved(ofMouseEventArgs & args) {
		this->updateCursorAndIndex();
		this->getElementUnderCursor();
	}
	
	//----------
	void Scene::mousePressed(ofMouseEventArgs & args) {
		this->updateCursorAndIndex();
		this->cursor.start(args.button);
		this->cursor.captured = true;
		
		this->getElementUnderCursor()->cursorDown(cursor);
		if (cursor.captured) {
			this->camera->setMouseActions(false);
		}
		this->lockIndex = true;
	}
	
	//----------
	void Scene::mouseReleased(ofMouseEventArgs & args) {
		this->updateCursorAndIndex();
		this->cursor.end(args.button);
		this->getElementUnderCursor()->cursorReleased(this->cursor);
		this->camera->setMouseActions(true);
		this->lockIndex = false;
	}
	
	//----------
	void Scene::mouseDragged(ofMouseEventArgs & args) {
		this->updateCursorAndIndex();
		this->getElementUnderCursor()->cursorDragged(this->cursor);
	}
	
	
	//----------
	void Scene::keyPressed(ofKeyEventArgs & args) {
		
	}
	
	//----------
	void Scene::keyReleased(ofKeyEventArgs & args) {
		
	}
}