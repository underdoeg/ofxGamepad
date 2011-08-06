#include "ofxGamepadHandler.h"

#ifdef USE_OIS
using namespace OIS;
InputManager* oisInputManager;
class tempPad{
public:
	tempPad(JoyStick* s){stick=s;handled=false;};
	JoyStick* stick;
	bool handled;
};
#endif

ofxGamepadHandler* ofxGamepadHandler::singleton;
bool ofxGamepadHandler::hasSingleton = false;

ofxGamepadHandler::ofxGamepadHandler():hasHotplug(false),hotplugNext(0)
{
	ofAddListener(ofEvents.update, this, &ofxGamepadHandler::update);
	ofAddListener(ofEvents.exit, this, &ofxGamepadHandler::exit);
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
		InputManager* inputManager=InputManager::createInputSystem(pl);
		
		gamepadList padsOld=gamepads;
		std::vector<tempPad> sticks;
		gamepads.clear();
		
		int numPads = inputManager->getNumberOfDevices(OISJoyStick);
		for( int i = 0; i < numPads; i++ )
		{
			JoyStick* js = (JoyStick*)inputManager->createInputObject(OISJoyStick, true );
			sticks.push_back(tempPad(js));
		}
		
		std::vector<tempPad>::iterator sIt = sticks.begin();
		while(sIt!=sticks.end()){
			gamepadList::iterator gIt = padsOld.begin();
			while(gIt!=padsOld.end()){
				if((*sIt).stick->vendor() == (*gIt)->name){
					ofPtr<ofxGamepadOIS> p = *gIt;
					p->updateJoystick((*sIt).stick);
					gamepads.push_back(p);
					padsOld.erase(gIt);
					(*sIt).handled = true;
					break;
				}
				++gIt;
			}
			++sIt;
		}
		
		sIt = sticks.begin();
		while(sIt!=sticks.end()){
			cout << (*sIt).handled << endl;
			if(!(*sIt).handled)
				gamepads.push_back(ofPtr<ofxGamepadOIS>(new ofxGamepadOIS((*sIt).stick)));
			++sIt;
		}

		oisInputManager = inputManager;
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

