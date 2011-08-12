#ifndef OFXGAMEPADHANDLER_H
#define OFXGAMEPADHANDLER_H

#include "ofMain.h"
#include "ofxGamepad.h"

#if defined(TARGET_OSX) || defined(TARGET_OS_WIN32)
#define USE_OIS
#endif

#if defined(TARGET_LINUX) && ! defined(USE_OIS)
#include "ofxGamepadLinux.h"
#endif

#ifdef USE_OIS
#include "OIS.h"
#include "ofxGamepadOIS.h"
typedef std::vector< ofPtr<ofxGamepadOIS> > gamepadList;
#else
typedef std::vector< ofPtr<ofxGamepad> > gamepadList;
#endif;


class ofxGamepadHandler: public ofThread {

public:
	~ofxGamepadHandler();
	static ofxGamepadHandler* get();

	int getNumPads();
	ofxGamepad* getGamepad(int num);

	void updatePadList();
	void update(ofEventArgs &arg);
	void update();
	void draw(int x, int y);
	void enableHotplug(int interval=1500); //interval in milliseconds the system looks for new gamepads
	
	ofEvent<ofxGamepadEvent> onGamepadPlug;
	ofEvent<ofxGamepadEvent> onGamepadUnplug;

	void threadedFunction();

private:
	void exit(ofEventArgs &arg);
	ofxGamepadHandler();

	gamepadList gamepads;
	gamepadList gamepadsNew;

	static ofxGamepadHandler* singleton;
	static bool hasSingleton;
	
	bool hasHotplug;
	int hotplugNext;
	int hotplugInterval;

	std::vector<int> activeIDs;
};

#endif // OFXGAMEPADHANDLER_H
