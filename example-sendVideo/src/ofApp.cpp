#include "ofApp.h"

ofApp::ofApp(){
	_videoGrabber.setup(640, 480);
	_ndiSender.setMetaData("ofxNDISender example", "sender example", "ofxNDI", "0.0.0", "", "", "");
}
void ofApp::update(){
	_videoGrabber.update();
	if(_videoGrabber.isFrameNew()){
		_ndiSender.send(_videoGrabber.getPixels());
	}
}
void ofApp::draw(){
	_videoGrabber.draw(0,0);
}
