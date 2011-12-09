#include "ofxGamepad.h"

int ofxGamepad::curID=0;

ofxGamepad::ofxGamepad():id(curID),axisMinVal(-32768),axisMaxVal(32767),isDisabled(false)
{
	curID++;
}

ofxGamepad::~ofxGamepad()
{
}

void ofxGamepad::axisChanged(int axis, int value)
{
	float val = ofMap(value, axisMinVal, axisMaxVal, -1, 1);
	if(val>-axisThreshold[axis].min && val<axisThreshold[axis].max)
		val = 0;
	axisValues[axis]=val;
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
	for(int i=0;i<axisValues.size();i++){
		axisValues[i] = 0;
	}
}

void ofxGamepad::setNumButtons(int amt)
{
	numButtons = amt;
	buttonValues.resize(numButtons);
	for(int i=0;i<buttonValues.size();i++){
		buttonValues[i] = false;
	}
}

void ofxGamepad::draw(int x, int y)
{
	int curX=0;
	int highestY;

	ofPushMatrix();
	ofTranslate(x, y);
	ofSetColor(255);
	ofPushMatrix();
	ofRotate(90);
	ofDrawBitmapString(name+" ("+ofToString(id)+")", 0, 0);
	ofPopMatrix();
	curX=17;

	int margin=3;
	ofRectangle axisSize(curX,0,85, 17);
	for(int i=0; i<getNumAxis(); i++) {
		ofSetColor(70);
		ofRect(axisSize);
		ofSetColor(255);
		float x =  ofMap(getAxisValue(i), -1, 1, axisSize.x, axisSize.width+axisSize.x);
		ofLine(x, axisSize.y, x, axisSize.y+axisSize.height);
		ofSetColor(20);
		ofDrawBitmapString(ofToString(i), axisSize.x, axisSize.y+axisSize.height-1);
		axisSize.y+=axisSize.height+margin;
		if(axisSize.y>highestY)
			highestY=axisSize.y;
	}

	curX+=axisSize.width+margin;
	ofRectangle btnSize(curX,0,17,17);
	for(int i=0; i<getNumButtons(); i++) {
		if(getButtonValue(i))
			ofSetColor(255);
		else
			ofSetColor(70);
		ofRect(btnSize);
		btnSize.y+=btnSize.height+margin;
		ofSetColor(20);
		ofDrawBitmapString(ofToString(i), btnSize.x, btnSize.y-4);
		if(btnSize.y>highestY)
			highestY=axisSize.y;
	}
	curX+=btnSize.width;

	ofPopMatrix();

	drawSize.x=curX;
	drawSize.y=highestY;
}

void ofxGamepad::setName(string n)
{
	name = n;
	if(name==PS3_NAME) {
		type = GAMEPAD_PS3;
	} else if(name==XBOX_NAME) {
		type = GAMEPAD_XBOX;
	} else {
		type = GAMEPAD_UNKNOWN;
	}
}

void ofxGamepad::disable(){
	isDisabled=true;
}

void ofxGamepad::setAxisThreshold(int axis, float thresh)
{
	setAxisThreshold(axis, thresh, thresh);
}

void ofxGamepad::setAxisThreshold(int axis, float min, float max)
{
	axisThreshold[axis].max = max;
	axisThreshold[axis].min = min;
}

