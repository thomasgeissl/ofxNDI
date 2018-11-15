#pragma once
#include "ofMain.h"
#include "ofxNDISender.h"

class ofApp : public ofBaseApp{
	public:
		ofApp();
		void update();
		void draw();

		ofxNDISender ndiSender;
		ofImage screen;
		ofPixels colorPixels;
		int width = 640;
		int height = 360;



		
	};
