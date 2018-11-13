#include "ofApp.h"

ofApp::ofApp(){
    ofSetFrameRate(60);
}
void ofApp::update(){
    _ndiGrabber.update();
    ofLogNotice("ofApp")<<"pixel at "<<_ndiGrabber.getPixels().getColor(100, 100);
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}
void ofApp::draw(){
    _ndiGrabber.draw(50,50, 800, 450);
}
