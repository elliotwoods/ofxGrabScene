//
//  Node.h
//  ofxGrabScene
//
#pragma once

#include "ofNode.h"
#include "ofParameterGroup.h"

#include "Assets.h"
#include "Cursor.h"

namespace GrabScene {
	class BaseNode {
	public:
		BaseNode();
		virtual void draw() = 0;
		virtual void drawStencil() = 0;
		virtual ofNode & getNode() = 0;
		
		virtual void cursorOver(Cursor &) { }
		virtual void cursorOut(Cursor &) { }
		
		void save(ofBuffer &);
		void load(ofBuffer &);
		
		string getName() const;
		void setName(const string &);
		
		void updateTranslate();
		void updateRotate();
		ofParameterGroup parameters;

	protected:
		void translateCallback(ofVec3f &);
		void rotateCallback(ofVec3f &);
		
		ofParameter<ofVec3f> translate;
		ofParameter<ofVec3f> rotate;
	};
	
	///This is the one you generally want to inherit from
	class Node : public ofNode, public BaseNode {
	public:
		virtual void draw() { ofNode::draw(); }
		virtual void drawStencil() { this->draw(); }
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
        NullNode();
		void draw() { };
		void drawStencil() { };
		ofNode & getNode() { return node; };
	protected:
		ofNode node;
	};
}