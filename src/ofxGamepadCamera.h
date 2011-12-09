#ifndef OFXGAMEPADCAMERA_H
#define OFXGAMEPADCAMERA_H

#include "ofxGamepadHandler.h"

class ofxGamepadCamera : public ofCamera {

public:
	ofxGamepadCamera();
	~ofxGamepadCamera();
	void setup();
	void setGamepad(ofxGamepad* pad);
	void setInitialPosition(float x, float y, float z);
	void setInitialPosition(ofVec3f pos);
	void reset();

	float speedRotation;
	float speedMove;
	bool useAnalogueDolly;
private:
	void update(ofEventArgs& e);
	float lastTime;
	ofxGamepad* pad;
	ofVec3f initialPosition;
};

#endif // OFXGAMEPADCAMERA_H
