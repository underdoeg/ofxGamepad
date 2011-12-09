#ifndef OFXGAMEPAD_H
#define OFXGAMEPAD_H

#include "ofMain.h"
#include "ofxGamepadConfigs.h"

class ofxGamepad;

class ofxGamepadThreshold{
public:
	ofxGamepadThreshold(){
		min = max = 0.05;
	}
	float min;
	float max;
};

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

	virtual void rumble(float level=1){};

	ofEvent<ofxGamepadAxisEvent> onAxisChanged;
	ofEvent<ofxGamepadButtonEvent> onButtonPressed;
	ofEvent<ofxGamepadButtonEvent> onButtonReleased;
	ofEvent<ofxGamepadEvent> onUnplug;

	void draw(int x, int y);
	virtual void exit(){};
	
	void disable();

	void setAxisThreshold(int axis, float thresh);
	void setAxisThreshold(int axis, float min, float max);

	ofPoint drawSize;
	string name;	
	int id;

	GAMEPAD_TYPE type;

protected:
	void axisChanged(int axis, int value);
	void buttonChanged(int button, bool value);
	void setNumAxis(int amount);
	void setNumButtons(int amount);
	void setName(string name);

	int axisMinVal;
	int axisMaxVal;
	bool isDisabled;

private:
	static int curID;
	int numAxis;
	int numButtons;
	std::vector<bool> buttonValues;
	std::vector<float> axisValues;
	std::map<int, ofxGamepadThreshold> axisThreshold;
};

#endif // OFXGAMEPAD_H
