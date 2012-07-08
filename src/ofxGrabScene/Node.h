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
		
		void save(ofBuffer &);
		void load(ofBuffer &);
	};
	
	class Node : public ofNode, public BaseNode {
	public:
		virtual void draw() { ofNode::draw(); }
		virtual void drawStencil() { ofNode::draw(); }
		ofNode & getNode() { return *this; }
	};
	
	class WrappedNode : public BaseNode {
	public:
		WrappedNode(ofNode &);
		virtual void draw();
		virtual void drawStencil();
		
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