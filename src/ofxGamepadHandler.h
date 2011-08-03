#ifndef OFXGAMEPADHANDLER_H
#define OFXGAMEPADHANDLER_H

#include "ofMain.h"
#include "ofxGamepad.h"

typedef std::vector< ofPtr<ofxGamepad> > gamepadList;

class ofxGamepadHandler {

public:
	~ofxGamepadHandler();
	static ofxGamepadHandler* get();

	void updatePadList();
	void update(ofEventArgs &arg);
	void draw(int x, int y);

private:
	void exit(ofEventArgs &arg);
	ofxGamepadHandler();

	gamepadList gamepads;

	static ofxGamepadHandler* singleton;
	static bool hasSingleton;

};

#endif // OFXGAMEPADHANDLER_H
