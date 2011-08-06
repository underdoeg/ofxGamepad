#ifndef OFXGAMEPADLINUX_H
#define OFXGAMEPADLINUX_H
#include "ofMain.h"

#ifdef TARGET_LINUX

#include "ofxGamepad.h" // Base class: ofxGamepad

class ofxGamepadLinux : public ofxGamepad
{

public:
	ofxGamepadLinux(string filename);
	~ofxGamepadLinux();

	void update();
	void exit();
	
private:
	int fd;
	string filename;
};

#endif //TARGET LINUX

#endif // OFXGAMEPADLINUX_H
