#include "ofApp.h"

ofApp::ofApp()
{
	// _videoGrabber.setup(640, 480);
	_ndiSender.setMetaData("ofxNDISender example", "sender example", "ofxNDI", "0.0.0", "", "", "");
	screen.allocate(360, 180, OF_IMAGE_COLOR_ALPHA);
}
void ofApp::update()
{
	// _videoGrabber.update();
	// if(_videoGrabber.isFrameNew()){

	_ndiSender.send(screen.getPixels());
	// }
}
void ofApp::draw()
{
	ofBackground(0, 0, 0);
	ofSetColor(255);
	ofDrawEllipse(ofGetWidth() / 2 + ofRandom(10), ofGetHeight() / 2 + ofRandom(10), 50, 50);
	screen.grabScreen(ofGetWidth()/2 - 180, ofGetHeight() / 2 - 90 , 360, 180);

}
