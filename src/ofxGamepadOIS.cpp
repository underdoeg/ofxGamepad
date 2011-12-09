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

ofxGamepadOIS::ofxGamepadOIS(InputManager* inputManager):ofxGamepad(){
	ofxGamepadOIS((JoyStick*)inputManager->createInputObject(OISJoyStick, true ));
};

ofxGamepadOIS::ofxGamepadOIS(OIS::JoyStick* js){
	updateJoystick(js);
	string msg=name;
	msg += ": "+ofToString(getNumAxis())+" axis";
	msg += ", "+ofToString(getNumButtons())+" buttons";
	ofLog(OF_LOG_NOTICE, msg);
};

ofxGamepadOIS::~ofxGamepadOIS(){

};

void ofxGamepadOIS::updateJoystick(OIS::JoyStick* js){
	isDisabled=false;
	joystick = js;
	setNumAxis(joystick->getNumberOfComponents(OIS_Axis));
	setNumButtons(joystick->getNumberOfComponents(OIS_Button));
	setName(joystick->vendor());
	joystick->setEventCallback(this);
	id=joystick->getID();
	
	forceFeedback = (OIS::ForceFeedback*)joystick->queryInterface( OIS::Interface::ForceFeedback );
	if( forceFeedback )
	{
		bFFFound = true;
		
		//cout << endl << " * Number of force feedback axes : " << forceFeedback->getFFAxesNumber() << endl;
		const OIS::ForceFeedback::SupportedEffectList &lstFFEffects = forceFeedback->getSupportedEffects();
		if (lstFFEffects.size() > 0)
		{
			stringstream msg; 
			msg << " * Supported effects :" << std::endl;
			ofLog(OF_LOG_VERBOSE, msg.str());
			
			OIS::ForceFeedback::SupportedEffectList::const_iterator itFFEff;
			for(itFFEff = lstFFEffects.begin(); itFFEff != lstFFEffects.end(); ++itFFEff)
			{
				stringstream msg; 
				msg << OIS::Effect::getEffectTypeName(itFFEff->second) << std::endl;
				ofLog(OF_LOG_VERBOSE, msg.str());
			}
		}
		else{
			stringstream msg; 
			msg << "No supported effect found!" << std::endl;
			ofLog(OF_LOG_WARNING, msg.str());
		}
	}else{
		stringstream msg; 
		msg << "No force feedback support detected!" << std::endl;
		ofLog(OF_LOG_NOTICE, msg.str());
		
		bFFFound = false;
	}
}

void ofxGamepadOIS::update(){
	if(isDisabled)
		return;
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

void ofxGamepadOIS::rumble(float level)
{
	if(bFFFound){
		forceFeedback->setMasterGain(level);
	}
}
