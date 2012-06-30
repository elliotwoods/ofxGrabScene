//
//  Node.h
//  ofxGrabScene
//
#pragma once

#include "ofNode.h"
#include "Assets.h"

namespace GrabScene {
	class BaseNode {
	public:
		virtual void draw() = 0;
		virtual void drawStencil() = 0;
		virtual ofNode & getNode() = 0;
	};
	
	class Node : public ofNode {
	public:
		ofNode & getNode() { return *this; }
	};
	
	class WrappedNode : public BaseNode {
	public:
		WrappedNode(ofNode &);
		void draw();
		void drawStencil();
		
		ofNode & getNode();		
	protected:
		ofNode * node;
	};
	
	class NullNode : public BaseNode {
	public:
		void draw() { };
		void drawStencil() { };
		ofNode & getNode() { return node; };
	protected:
		ofNode node;
	};
}