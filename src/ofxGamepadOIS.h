/*
 *  ofxGamepadOIS.h
 *  ofxGamepadExample
 *
 *  Created by Philip Whitfield on 8/5/11.
 *  Copyright 2011 undef.ch. All rights reserved.
 *
 */

#ifndef _CLASS_ofxGamepadOIS_
#define _CLASS_ofxGamepadOIS_

#include "OIS.h"
#include "ofxGamepad.h"

class ofxGamepadOIS: public ofxGamepad, public OIS::JoyStickListener{
public:
	ofxGamepadOIS(OIS::InputManager* inputManager);
	ofxGamepadOIS(OIS::JoyStick* joystick);
	~ofxGamepadOIS();
    void updateJoystick(OIS::JoyStick*);
	void update();
	void rumble(float level=1);
		
private:
	bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
	bool buttonReleased( const OIS::JoyStickEvent &arg, int button );
	bool axisMoved( const OIS::JoyStickEvent &arg, int axis );
	bool povMoved( const OIS::JoyStickEvent &arg, int pov );
	bool vector3Moved( const OIS::JoyStickEvent &arg, int index);

	OIS::JoyStick* joystick;

	OIS::ForceFeedback* forceFeedback;
	bool bFFFound;
};

#endif
