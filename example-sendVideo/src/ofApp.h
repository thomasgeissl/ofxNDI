#pragma once
#include "ofMain.h"
#include "ofxNDISender.h"

class ofApp : public ofBaseApp{
	public:
		ofApp();
		void update();
		void draw();

		ofxNDISender _ndiSender;
		ofVideoGrabber _videoGrabber;
};
