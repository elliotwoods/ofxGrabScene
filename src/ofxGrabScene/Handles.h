//
//  Handles.h
//  ofxGrabScene
//
#pragma once

#include "Element.h"
#include "Node.h"
#define GRABSCENE_HANDLES_RADIUS_1 1.0f
#define GRABSCENE_HANDLES_RADIUS_2 0.1f

namespace GrabScene {
	class Node;
	
	class Handles {
	public:
		enum Axis { X_AXIS, Y_AXIS, Z_AXIS, NO_AXIS };
		
		class BaseHandle : public Element {
		public:
			BaseHandle(Axis);
			ofVec3f getDirection() const;
			static void setParent(Node * node);
			void cursorOver(const Cursor &);
			void cursorOut(const Cursor &);
		protected:
			Axis axis;
			static Node * parent;
			ofColor color;
			
			void rotateAxis() const;
			void setStyleFill() const;
			void setStyleLine() const;
		private:
			bool rollover;
		};
		
		class Translate : public BaseHandle {
		public:
			Translate(Axis);
			static void init();
			
			void draw() const;
			void drawStencil() const;
			
			void cursorDragged(const MovingCursor &);
			
		protected:
			static ofMesh fill;
			static ofMesh line;
		};
		
		class Rotate : public BaseHandle {
		public:
			Rotate(Axis);
			static void init();
			
			void draw() const;
			void drawStencil() const;
			
			void cursorDragged(const MovingCursor &);
			
		protected:
			static ofMesh fill;
			static ofMesh line;
		};
		
		Handles();
		void init();
		Translate translateX, translateY, translateZ, translateC;
		Rotate rotateX, rotateY, rotateZ;
	};
}