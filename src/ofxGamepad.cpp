#include "ofxGamepad.h"

ofxGamepad::ofxGamepad():axisMinVal(-32767),axisMaxVal(32767)
{
}

ofxGamepad::~ofxGamepad()
{
}

void ofxGamepad::axisChanged(int axis, int value)
{
	axisValues[axis]=ofMap(value, axisMinVal, axisMaxVal, -1, 1);
	static ofxGamepadAxisEvent evt;
	evt.gamepad=this;
	evt.axis=axis;
	evt.value=axisValues[axis];
	ofNotifyEvent(onAxisChanged, evt);
}

void ofxGamepad::buttonChanged(int button, bool value)
{
	buttonValues[button]=value;
	value?buttonPressed(button):buttonReleased(button);
}

void ofxGamepad::buttonPressed(int button)
{
	static ofxGamepadButtonEvent evt;
	evt.gamepad=this;
	evt.button=button;
	evt.value=1;
	ofNotifyEvent(onButtonPressed, evt);
}

void ofxGamepad::buttonReleased(int button)
{
	static ofxGamepadButtonEvent evt;
	evt.gamepad=this;
	evt.button=button;
	evt.value=0;
	ofNotifyEvent(onButtonReleased, evt);
}

bool ofxGamepad::getButtonValue(int button)
{
	return buttonValues[button];
}

float ofxGamepad::getAxisValue(int axis)
{
	return axisValues[axis];
}

float ofxGamepad::getAxisValueU(int axis)
{
	return (axisValues[axis]+1)*.5;
}

int ofxGamepad::getNumAxis()
{
	return numAxis;
}

int ofxGamepad::getNumButtons()
{
	return numButtons;
}

void ofxGamepad::setNumAxis(int amt)
{
	numAxis = amt;
	axisValues.resize(numAxis);
}

void ofxGamepad::setNumButtons(int amt)
{
	numButtons = amt;
	buttonValues.resize(numButtons);
}

