#ifndef OFXGAMEPADLINUX_H
#define OFXGAMEPADLINUX_H

#include "ofMain.h"

#ifdef TARGET_LINUX

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include "ofxGamepad.h" // Base class: ofxGamepad


#define BITS_PER_LONG (sizeof(long) * 8)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)    ((array[LONG(bit)] >> OFF(bit)) & 1)

#define N_EFFECTS 6

static char* effect_names[] = {
	"Sine vibration",
	"Constant Force",
	"Spring Condition",
	"Damping Condition",
	"Strong Rumble",
	"Weak Rumble"
};

class ofxGamepadLinux : public ofxGamepad
{

public:
	ofxGamepadLinux(string filename);
	~ofxGamepadLinux();
	void update();
	void exit();
	void setupFF();
	void playFF(int i);
	void stopAllFF();
	void rumble(float level=1);
private:
	int fd;
	int fdEvent;
	string filename;
	//force feedback variables
	struct ff_effect effects[N_EFFECTS];
	struct input_event play, stop;
	unsigned long features[4];
	int n_effects;	/* Number of effects the device can play at the same time */
};

#endif //TARGET LINUX

#endif // OFXGAMEPADLINUX_H
