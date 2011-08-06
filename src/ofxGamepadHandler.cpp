#include "ofxGamepadHandler.h"

#ifdef USE_OIS

using namespace OIS;

InputManager* oisInputManager;

#endif

ofxGamepadHandler* ofxGamepadHandler::singleton;
bool ofxGamepadHandler::hasSingleton = false;

ofxGamepadHandler::ofxGamepadHandler():hasHotplug(false),hotplugNext(0)
{
	ofAddListener(ofEvents.update, this, &ofxGamepadHandler::update);
	ofAddListener(ofEvents.exit, this, &ofxGamepadHandler::exit);
#ifdef USE_OIS
	
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

void ofxGamepadHandler::enableHotplug(int interval){
	hasHotplug=true;
	hotplugInterval=interval;
}

void ofxGamepadHandler::updatePadList()
{
#ifdef USE_OIS
	try
	{
		if(oisInputManager != NULL){ 
			oisInputManager->destroyInputSystem(oisInputManager);
		}
		ParamList pl;
		oisInputManager=InputManager::createInputSystem(pl);
		
		int numPads = oisInputManager->getNumberOfDevices(OISJoyStick);
		for( int i = 0; i < numPads; i++ )
		{
			JoyStick* js = (JoyStick*)oisInputManager->createInputObject(OISJoyStick, true );
			if (i<gamepads.size()) {
				gamepads[i]->updateJoystick(js);
			}else{
				gamepads.push_back(ofPtr<ofxGamepadOIS>(new ofxGamepadOIS(js)));
			}
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
	if(hasHotplug){
		int elapsed=ofGetElapsedTimeMillis();
		if(elapsed>hotplugNext){
			updatePadList();
			hotplugNext=elapsed+hotplugInterval;
		}
	}
	
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

