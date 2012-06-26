//
//  Handles.h
//  ofxGrabScene-example
//
#pragma once

#include "ofMesh.h"
#include "ofGraphics.h"
#include "ofEvents.h"

#include "Element.h"
#include "Node.h"

namespace GrabScene {
	class Node;
	
	class Handles {
		enum Axis {X_AXIS, Y_AXIS, Z_AXIS};
		
	public:
		class BaseHandle : public Element {
		public:
			static void setParent(Node *);
		protected:
			static Node * parent;
		};
		
		class Translate : public BaseHandle {
		public:
			Translate(Axis);
			static void init();
			
			void draw() const;
			void drawStencil() const;
			
			void cursorDragged(const MovingCursor &);
			
			ofVec3f getDirection() const;
		protected:
			static ofMesh fill;
			static ofMesh line;
			
			void begin() const;

			static bool initialised;

			Axis axis;
			ofColor color;
		};
		
		Handles();
		void init();
		Translate translateX, translateY, translateZ;
	};
}