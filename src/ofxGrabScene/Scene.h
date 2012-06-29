//
//  Scene.h
//  ofxGrabScene
//

#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"

#include "Node.h"
#include "Element.h"
#include "Cursor.h"
#define GRABSCENE_ON_TOP_SCALE 0.1f
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
		
		// These functions return 0 if nothing found
		bool hasValidSelection() const;
		Node * const getSelectedNode() const;
		void setSelectedNode(Node &);
		
		Element * const getElementUnderCursor();
		
		ofFbo & getIndexBuffer();
		const MovingCursor & getCursor();
	protected:
		
		////
		//graph
		//
		///Nodes are important for selection and handles
		vector<Node*> nodes;
		
		///Elements are imporant for any on-screen controls
		vector<Element*> elements;
		
		Node * selection;
		
		typedef vector<Node*>::iterator node_iterator;
		typedef vector<Node*>::const_iterator const_node_iterator;
		typedef vector<Element*>::iterator element_iterator;
		typedef vector<Element*>::const_iterator const_element_iterator;
		//
		////
		
		
		////
		//general
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
		void drawFrameBuffer(ofFbo &);
		ofFbo frameBuffer;
		////
		
		
		////
		//element index buffer
		//
		ofFbo indexBuffer;
		bool lockIndex;
		unsigned short index;
		int indexCachedFrame;
		void updateCursorAndIndex();
		void updateCursorOnly();
		void updateCursor();
		//
		////
		
		
		////
		//node index buffer
		//
		ofFbo nodeIndexBuffer;
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
		//
		////
	};
}