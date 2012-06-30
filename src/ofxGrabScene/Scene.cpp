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
		this->selectedNode = 0;
		
		this->elements.push_back(new NullElement());
		this->nodes.push_back(new NullNode());
		
		indexBuffer.allocate(512, 512, GL_RGB32F);

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
		
		AssetRegister.init();
		handles.init();
		
	}
	
	//----------
	void Scene::draw() {
		
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
		
		//not onTop
		const_element_iterator it;
		for (it = elements.begin(); it != elements.end(); it++) {
			if (!(**it).onTop())
				(**it).draw();
		}
		
		//onTop
		if (frameBuffer.getWidth() != ofGetWidth() || frameBuffer.getHeight() != ofGetHeight()) {
			frameBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 4);
		}
		frameBuffer.bind();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (it = elements.begin(); it != elements.end(); it++) {
			if ((**it).onTop())
				(**it).draw();
		}
		frameBuffer.unbind();
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		shader("constant").begin();
		shader("constant").setUniformTexture("tex", frameBuffer, 0);
		drawFullscreen(frameBuffer);
		shader("constant").end();
		//
		////
		
		
		////
		//nodeindexbuffer
		if (nodeIndexBuffer.getWidth() != ofGetWidth() || nodeIndexBuffer.getHeight() != ofGetHeight()) {
			nodeIndexBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);
		}
		nodeIndexBuffer.bind();
		ofClear(0,0,0,0);
		shader("index").begin();
		int index = 0;
		for (itN = this->nodes.begin(); itN != this->nodes.end(); itN++) {
			shader("index").setUniform1i("index", index);
			(*itN)->drawStencil();
			index++;
		}
		shader("index").end();
		
		GLfloat reading[3];
		glReadPixels(ofGetMouseX(), ofGetHeight() - 1 - ofGetMouseY(), 1, 1, GL_RGB, GL_FLOAT, &reading);
		this->nodeUnderCursor =  reading[0] * GRABSCENE_INDEX_VALUE_SCALE;
		
		nodeIndexBuffer.unbind();
		
		ofShader & outlineShader(shader("outlineIndex"));
		outlineShader.begin();
		outlineShader.setUniform1i("selection", this->selectedNode);
		outlineShader.setUniform1i("hover", this->nodeUnderCursor);
		outlineShader.setUniform1f("valueOffset", 1 << 10);
		outlineShader.setUniformTexture("texIndex", nodeIndexBuffer, 2);
		drawFullscreen(nodeIndexBuffer);
		outlineShader.end();
		//
		////
		
		ofPopStyle();
		
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
		BaseNode * newNode = new WrappedNode(node);
		this->nodes.push_back(newNode);
	}

	//----------
	bool Scene::hasSelection() const {
		return (this->selectedNode != 0);
	}
	
	//----------
	BaseNode & Scene::getSelectedNode() {
		if (this->selectedNode >= this->nodes.size()) {
			ofLogWarning("GrabScene") << "Selected node index is outside bounds of available nodes, selecting null node";
			this->selectedNode = 0;
		}
		return *this->nodes[this->selectedNode];
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
	void Scene::setSelectedNode(const unsigned int index) {
		if (index >= this->nodes.size()) {
			ofLogError("GrabScene") << "Node index " << index << " is outside the range of available nodes, selecting null node";
		} else {
			this->selectedNode = index;
			Handles::BaseHandle::setParent(this->nodes[index]);
			if (this->selectedNode == 0)
				Handles::BaseHandle::disable();
			else
				Handles::BaseHandle::enable();
		}
	}
	
	//----------
	Element & Scene::getElementUnderCursor() {
		if (this->index >= this->elements.size()) {
			ofLogError("ofxGrabScene") << "Found index " << this->index << " is out of range for this scene, which has " << this->elements.size() << " registered elements. Selecting null element.";
			this->index = 0;
		}
			
		return *this->elements[this->index];
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
		
		ofShader & indexShader(AssetRegister.getShader("index"));
		indexShader.begin();
		indexShader.setUniform1f("valueOffset", GRABSCENE_INDEX_VALUE_SCALE);
		
		//standard
		int index = 0;
		for (int i=0; i<elements.size(); i++) {
			if (elements[i]->onTop())
				continue;
			indexShader.setUniform1i("index", index++);
			elements[i]->drawStencil();
		}
		//onTop
		for (int i=0; i<elements.size(); i++) {
			if (!elements[i]->onTop())
				continue;
			indexShader.setUniform1i("index", index++);
			elements[i]->drawStencil();
		}
		
		indexShader.end();
		ofPopStyle();
		
		ofPopMatrix();
		
		updateCursor();
		glDisable(GL_DEPTH_TEST);
		
		if (!this->lockIndex) {
			float rawValue[3];
			glReadPixels(0, 0, 1, 1, GL_RGB, GL_FLOAT, &rawValue);
			unsigned short readIndex = rawValue[0] * float(GRABSCENE_INDEX_VALUE_SCALE) + 0.5f;
			indexCachedFrame = ofGetFrameNum();
			
			//perform cursor over, cursor out actions
			if (this->index != readIndex) {
				this->getElementUnderCursor().cursorOut(cursor);
				this->index = readIndex;
				this->getElementUnderCursor().cursorOver(cursor);
			}
		}
		indexBuffer.end();
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
		//
		///

		this->cursor.lastUpdate = ofGetFrameNum();
	}
	
	//----------
	void Scene::update(ofEventArgs & args) {
		this->updateCursorAndIndex();
		
		if (this->hasSelection()) {
			float distanceToNode = (this->camera->getPosition() - this->getSelectedNode().getNode().getPosition()).length();
			float planeHeight = tan(camera->getFov() * DEG_TO_RAD / 2.0f) * 2.0f * distanceToNode;
			Handles::BaseHandle::setScale(planeHeight / 8.0f);
		}
	}
	
	
	//----------
	void Scene::mouseMoved(ofMouseEventArgs & args) {
		this->updateCursorAndIndex();
		this->getElementUnderCursor();
		
		//help ofxGrabCam by using our version of the world cursor
//		if (this->cursor.world != ofVec3f(0,0,0))
//			this->camera->setCursorWorld(this->cursor.world);
	}
	
	//----------
	void Scene::mousePressed(ofMouseEventArgs & args) {
		this->updateCursorAndIndex();
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
		this->updateCursorAndIndex();
		this->cursor.end(args.button);
		this->getElementUnderCursor().cursorReleased(this->cursor);
		this->camera->setMouseActions(true);
		this->lockIndex = false;
		
		//click
		if (!this->cursor.dragged && this->index==0) {
			this->setSelectedNode(nodeUnderCursor);
		}
	}
	
	//----------
	void Scene::mouseDragged(ofMouseEventArgs & args) {
		if (this->cursor.captured) {
			this->updateCursorAndIndex();
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