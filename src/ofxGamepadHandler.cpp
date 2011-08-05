#include "ofxGamepadHandler.h"

#if defined(TARGET_OSX) || defined(TARGET_OS_WIN32)
#define USE_OIS
#endif

#if defined(TARGET_LINUX) && ! defined(USE_OIS)
#include "ofxGamepadLinux.h"
#endif

#ifdef USE_OIS
#include "OIS.h"
#include "ofxGamepadOIS.h"

using namespace OIS;

InputManager* oisInputManager;

#endif

ofxGamepadHandler* ofxGamepadHandler::singleton;
bool ofxGamepadHandler::hasSingleton = false;

ofxGamepadHandler::ofxGamepadHandler()
{
	ofAddListener(ofEvents.update, this, &ofxGamepadHandler::update);
	ofAddListener(ofEvents.exit, this, &ofxGamepadHandler::exit);
#ifdef USE_OIS
	ParamList pl;
	oisInputManager=InputManager::createInputSystem(pl);
#endif
	updatePadList();
}

ofxGamepadHandler::~ofxGamepadHandler()
{
}

ofxGamepadHandler* ofxGamepadHandler::get()
{
	if(!hasSingleton) {
		singleton=new ofxGamepadHandler();
		hasSingleton=true;
	}
	return singleton;
}

void ofxGamepadHandler::updatePadList()
{
#ifdef USE_OIS
	try
	{
		int numSticks = oisInputManager->getNumberOfDevices(OISJoyStick);
		for( int i = 0; i < numSticks; ++i )
		{
			gamepads.push_back(ofPtr<ofxGamepad>(new ofxGamepadOIS(oisInputManager)));
		}
	}
	catch(OIS::Exception &ex)
	{
		stringstream msg; 
		msg << "\nException raised on joystick creation: " << ex.eText << std::endl;
		ofLog(OF_LOG_ERROR, msg.str());
	}
#elif defined(TARGET_LINUX)
	//check for joysticks
	ofFile file;
	for(int i=0; i<32; i++) {
		file.open("/dev/input/js"+ofToString(i));
		if(file.exists()) {
			try {
				gamepads.push_back(ofPtr<ofxGamepad>(new ofxGamepadLinux(file.getAbsolutePath())));
			} catch(std::exception& err) {
			}
		}
	}
#else
	ofLog(OF_LOG_ERROR, "ofxGamepad says: sorry, looks your system is not supported...");
#endif
}

void ofxGamepadHandler::update(ofEventArgs &args){
	gamepadList::iterator it=gamepads.begin();
	while(it!=gamepads.end()){
		(*it)->update();
		++it;
	}
}

void ofxGamepadHandler::draw(int x, int y)
{
	ofPushMatrix();
	ofTranslate(x, y);
	ofPoint offset(x, y);

	gamepadList::iterator it=gamepads.begin();
	while(it!=gamepads.end()){
		(*it)->draw(offset.x, offset.y);
		offset+=ofPoint((*it)->drawSize.x+20, 0);
		++it;
	}

	ofPopMatrix();
}

void ofxGamepadHandler::exit(ofEventArgs& arg)
{
	gamepadList::iterator it=gamepads.begin();
	while(it!=gamepads.end()){
		(*it)->exit();
		++it;
	}
	delete this;
}

ofxGamepad* ofxGamepadHandler::getGamepad(int num)
{
	return gamepads[num].get();
}

int ofxGamepadHandler::getNumPads()
{
	return gamepads.size();
}

