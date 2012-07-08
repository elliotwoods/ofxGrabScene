//
//  Scene.h
//  ofxGrabScene
//

#pragma once

#include <stdint.h>

#include "ofMain.h"
#include "ofxGrabCam.h"

#include "Node.h"
#include "Element.h"
#include "Cursor.h"
#include "Assets.h"
#include "Handles.h"

namespace GrabScene{
	class Scene : public ofNode {
	public:
		Scene();
		void init(ofxGrabCam & camera);
		void draw();
		
		void add(Element *);
		void add(Element & element) {
			this->add(&element);
		}
		void add(ofNode & node);
		void add(BaseNode * const node);
		
		bool hasSelection() const;
		BaseNode & getSelectedNode();
		void setSelectedNode(BaseNode &);
		void setSelectedNode(const uint16_t index);
		
		Element & getElementUnderCursor();
		
		ofFbo & getIndexBuffer();
		const MovingCursor & getCursor();
		
		ofEvent<const unsigned int> selectionChanged;
	protected:
		
		////
		//graph
		//
		///Nodes are important for selection and handles
		vector<BaseNode*> nodes;
		
		///Elements are imporant for any on-screen controls
		vector<Element*> elements;
		
		typedef vector<BaseNode*>::iterator node_iterator;
		typedef vector<BaseNode*>::const_iterator const_node_iterator;
		typedef vector<Element*>::iterator element_iterator;
		typedef vector<Element*>::const_iterator const_element_iterator;
		//
		////
		
		
		////
		//general
		bool initialised;
		ofRectangle viewport;
		ofMatrix4x4 viewMatrix;
		GLdouble viewDoubles[16];
		ofMatrix4x4 projectionMatrix;
		GLdouble projectionDoubles[16];
		ofxGrabCam * camera;
		GrabScene::MovingCursor cursor;
		//
		////
		
		
		////
		//graphics
		//
		void drawFullscreen(ofBaseHasTexture &);
		ofFbo frameBuffer;
		//
		////
		

		////
		//index buffer
		ofFbo indexBuffer;
		// 0 = no selection for the below
		uint16_t elementUnderCursor; 
		uint16_t nodeUnderCursor; 
		uint16_t nodeSelected;
		//
		bool lockElementIndex;
		//
		////

		
		////
		//cursor
		//
		void updateCursor();
		//
		////
		
		
		////
		//node interaction
		//
		Handles handles;
		//
		////
		
		
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
		void	assetsLoad(Assets &);
		//
		////
	};
}