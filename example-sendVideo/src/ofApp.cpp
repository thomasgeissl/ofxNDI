#include "ofApp.h"

ofApp::ofApp()
{
	ndiSender.setMetaData("ofxNDISender example", "sender example", "ofxNDI", "0.0.0", "", "", "");
	colorPixels.allocate(width, height, OF_PIXELS_RGB);
	screen.allocate(width, height, OF_IMAGE_COLOR);
	// color alpha pixels, use x to control alpha
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			colorPixels.setColor(x, y, ofColor(ofRandom(255), 133, 200));
		}
	}
}
void ofApp::update()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			colorPixels.setColor(x, y, ofColor(ofRandom(255), 133, 200));
		}
	}
	ndiSender.send(colorPixels);
	screen.setFromPixels(colorPixels);
}
void ofApp::draw()
{
	ofBackground(0, 0, 0);
	screen.draw(ofGetWidth() / 2 - width / 2, ofGetHeight() / 2 - height / 2);
}
