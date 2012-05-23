#include "ofxGamepadCamera.h"

ofxGamepadCamera::ofxGamepadCamera():pad(NULL),speedRotation(70), speedMove(200), useAnalogueDolly(false) {
}

ofxGamepadCamera::~ofxGamepadCamera() {
}

void ofxGamepadCamera::setup() {
	//CHECK IF THERE IS A GAMEPAD CONNECTED
	if(ofxGamepadHandler::get()->getNumPads()>0) {
		setGamepad(ofxGamepadHandler::get()->getGamepad(0));
	} else
		ofLogWarning("no gamepad connected in ofxGamepadCAmera::setup");

	ofAddListener(ofEvents().update, this, &ofxGamepadCamera::update);
}

void ofxGamepadCamera::update(ofEventArgs& e) {
	if(pad == NULL)
		return;

	float curTime = ofGetElapsedTimef();

	float mult = curTime - lastTime;
	pan(-pad->getAxisValue(PS3_STICK_R_X)*speedRotation*mult);
	tilt(-pad->getAxisValue(PS3_STICK_R_Y)*speedRotation*mult);

	truck(pad->getAxisValue(PS3_STICK_L_X)*speedMove*mult);
	boom(-pad->getAxisValue(PS3_STICK_L_Y)*speedMove*mult);

	if(useAnalogueDolly) {
		dolly(-(pad->getAxisValueU(PS3_THRUST_L2)-.5)*speedMove*mult);
		dolly((pad->getAxisValueU(PS3_THRUST_R2)-.5)*speedMove*mult);
	} else {
		if(pad->getButtonValue(PS3_BTN_L2)) {
			dolly(-speedMove*2*mult);
		}
		if(pad->getButtonValue(PS3_BTN_R2)) {
			dolly(speedMove*2*mult);
		}
	}

	if(pad->getButtonValue(PS3_BTN_L1)) {
		roll(-speedRotation*.7*mult);
	}
	if(pad->getButtonValue(PS3_BTN_R1)) {
		roll(speedRotation*.7*mult);
	}

	if(pad->getButtonValue(PS3_BTN_SELECT))
		reset();

	lastTime = curTime;
}

void ofxGamepadCamera::setGamepad(ofxGamepad* p) {
	pad = p;
}

void ofxGamepadCamera::setInitialPosition(float x, float y, float z) {
	setInitialPosition(ofVec3f(x, y, z));
}

void ofxGamepadCamera::setInitialPosition(ofVec3f pos) {
	initialPosition = pos;
}

void ofxGamepadCamera::reset() {
	setPosition(initialPosition);
	setOrientation(ofVec3f(0, 0, 0));
}
