//
//  Handles.h
//  ofxGrabScene
//
#pragma once

#include "ofCamera.h"

#include "Element.h"
#include "Node.h"
#include "Assets.h"

#define GRABSCENE_HANDLES_RADIUS_1 1.0f
#define GRABSCENE_HANDLES_RADIUS_2 0.08f
#define GRABSCENE_HANDLES_SATURATION 200
#define GRABSCENE_HANDLES_LINE_BRIGHTNESS 50
#define GRABSCENE_HANDLES_FILL_BRIGHTNESS 200

namespace GrabScene {
	class Handles {
	public:
		enum Axis { X_AXIS, Y_AXIS, Z_AXIS, NO_AXIS };
		
		class BaseHandle : public Element {
		public:
			BaseHandle(Axis);
			ofVec3f getDirection() const;
			static void setParent(BaseNode * node);
			static void setCamera(ofCamera &);
			void cursorOver(const Cursor &);
			void cursorOut(const Cursor &);
			bool onTop() const { return true; }
			static void setScale(float scale);
			static void enable();
			static void disable();
		protected:
			Axis axis;
			static BaseNode * parent;
			static ofCamera * camera;
			ofColor lineColor;
			ofColor fillColor;
			bool rollover;
			static float scale;
			static bool enabled;
			
			void rotateAxis() const;
			void setStyleFill() const;
			void setStyleLine() const;
		private:
			virtual string getReading() const = 0;
		};
		
		class Translate : public BaseHandle {
		public:
			Translate(Axis);
			static void init();
			
			void draw() const;
			void drawStencil() const;
			
			void cursorDragged(const MovingCursor &);
			
		protected:
			string getReading() const;
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
			ofMatrix4x4 doTransform() const;
			string getReading() const;
			static ofMesh fill;
			static ofMesh line;
		};
		
		Handles();
		void init(ofCamera &);
		Translate translateX, translateY, translateZ, translateC;
		Rotate rotateX, rotateY, rotateZ;
	};
}