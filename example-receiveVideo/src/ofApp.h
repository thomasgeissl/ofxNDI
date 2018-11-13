#pragma once
#include "ofMain.h"
#include "ofxNDIGrabber.h"

class ofApp : public ofBaseApp
{
	public:
		ofApp();
		void update();
		void draw();

		ofxNDIGrabber _ndiGrabber;
};
