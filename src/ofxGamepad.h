#ifndef OFXGAMEPAD_H
#define OFXGAMEPAD_H

#include "ofMain.h"

class ofxGamepad;

class ofxGamepadEvent
{
public:
	ofxGamepad* gamepad;
};

class ofxGamepadButtonEvent: public ofxGamepadEvent
{
public:
	int button;
	bool value;
};

class ofxGamepadAxisEvent: public ofxGamepadEvent
{
public:
	int axis;
	float value;
	float valueRel;
};

class ofxGamepad
{

public:
	ofxGamepad();
	~ofxGamepad();
	virtual void update() {};

	void buttonReleased(int button);
	void buttonPressed(int button);

	bool getButtonValue(int button);
	float getAxisValue(int axis);
	float getAxisValueU(int axis);

	int getNumAxis();
	int getNumButtons();

	ofEvent<ofxGamepadAxisEvent> onAxisChanged;
	ofEvent<ofxGamepadButtonEvent> onButtonPressed;
	ofEvent<ofxGamepadButtonEvent> onButtonReleased;

	void draw(int x, int y);
	virtual void exit(){};

	string name;
protected:
	void axisChanged(int axis, int value);
	void buttonChanged(int button, bool value);
	void setNumAxis(int amount);
	void setNumButtons(int amount);



	int axisMinVal;
	int axisMaxVal;

private:
	int numAxis;
	int numButtons;
	std::vector<bool> buttonValues;
	std::vector<float> axisValues;
};

#endif // OFXGAMEPAD_H
