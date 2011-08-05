/*
 *  ofxGamepadOIS.cpp
 *  ofxGamepadExample
 *
 *  Created by Philip Whitfield on 8/5/11.
 *  Copyright 2011 undef.ch. All rights reserved.
 *
 */

#include "ofxGamepadOIS.h"

using namespace OIS;

ofxGamepadOIS::ofxGamepadOIS(InputManager* inputManager){
	joystick = (JoyStick*)inputManager->createInputObject(OISJoyStick, true );
	setNumAxis(joystick->getNumberOfComponents(OIS_Axis));
	setNumButtons(joystick->getNumberOfComponents(OIS_Button));
	setName(joystick->vendor());
	joystick->setEventCallback(this);
};

ofxGamepadOIS::~ofxGamepadOIS(){

};

void ofxGamepadOIS::update(){
	joystick->capture();
}

bool ofxGamepadOIS::buttonPressed( const OIS::JoyStickEvent &arg, int button ){
	buttonChanged(button, 1);
	return true;
};
bool ofxGamepadOIS::buttonReleased( const OIS::JoyStickEvent &arg, int button ){
	buttonChanged(button, 0);
	return true;
};
bool ofxGamepadOIS::axisMoved( const OIS::JoyStickEvent &arg, int axis ){
	axisChanged(axis, arg.state.mAxes[axis].abs);
	return true;
};
bool ofxGamepadOIS::povMoved( const OIS::JoyStickEvent &arg, int pov ){
	return true;
};
bool ofxGamepadOIS::vector3Moved( const OIS::JoyStickEvent &arg, int index){
	return true;
};