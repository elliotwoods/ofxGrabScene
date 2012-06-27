//
//  Element.h
//  ofxGrabScene
//
#pragma once
#include "Cursor.h"

namespace GrabScene {
	class Element {
	public:
		virtual void draw() const = 0;
		virtual void drawStencil() const = 0;
		
		virtual void cursorOver(const Cursor &) { };
		virtual void cursorOut(const Cursor &) { };
		
		virtual void cursorDown(Cursor & cursor) { }
		virtual void cursorReleased(const MovingCursor &) { }
		
		virtual void cursorDragged(const MovingCursor &) { }
		
		virtual bool onTop() const { return false; }
	};
	
	class NullElement : public Element {
	public:
		void draw() const { }	
		void drawStencil() const { }
		void cursorDown(Cursor & cursor) { cursor.captured = false; }
	};
}