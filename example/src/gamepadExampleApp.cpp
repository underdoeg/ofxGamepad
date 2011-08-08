#include "gamepadExampleApp.h"

//--------------------------------------------------------------
void gamepadExampleApp::setup(){
	ofSetWindowTitle("gamepadExampleApp");
	ofBackground(0,0,0);

	ofSetFrameRate(120);
	ofSetLogLevel(OF_LOG_VERBOSE);

	//ofxGamepadHandler::get()->enableHotplug();
	ofxGamepadHandler::get()->getGamepad(0)->rumble(1);
}

//--------------------------------------------------------------
void gamepadExampleApp::update(){
}

//--------------------------------------------------------------
void gamepadExampleApp::draw(){
	ofxGamepadHandler::get()->draw(10,10);
}

//--------------------------------------------------------------
void gamepadExampleApp::keyPressed(int key){

}

//--------------------------------------------------------------
void gamepadExampleApp::keyReleased(int key){

}

//--------------------------------------------------------------
void gamepadExampleApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void gamepadExampleApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void gamepadExampleApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void gamepadExampleApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void gamepadExampleApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void gamepadExampleApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void gamepadExampleApp::dragEvent(ofDragInfo dragInfo){ 

}
