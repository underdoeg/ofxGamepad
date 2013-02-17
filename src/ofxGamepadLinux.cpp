#include "ofxGamepadLinux.h"

#ifdef TARGET_LINUX

#include <fcntl.h>
#include <linux/joystick.h>

ofxGamepadLinux::ofxGamepadLinux(string f):ofxGamepad() {
	filename=f;

	if ((fd = open(filename.c_str(), O_RDONLY)) < 0) {
		std::ostringstream str;
		str << filename << ": " << strerror(errno);
		throw std::runtime_error(str.str());
	} else {
		// ok
		uint8_t num_axis   = 0;
		uint8_t num_button = 0;
		ioctl(fd, JSIOCGAXES,    &num_axis);
		ioctl(fd, JSIOCGBUTTONS, &num_button);
		setNumAxis(num_axis);
		setNumButtons(num_button);

		// Get Name
		char name_c_str[1024];
		if (ioctl(fd, JSIOCGNAME(sizeof(name_c_str)), name_c_str) < 0) {
			std::ostringstream str;
			str << filename << ": " << strerror(errno);
			throw std::runtime_error(str.str());
		} else {
			name = name_c_str;
		}

		fcntl( fd, F_SETFL, O_NONBLOCK );
	}

	string msg=name;
	msg += ": "+ofToString(getNumAxis())+" axis";
	msg += ", "+ofToString(getNumButtons())+" buttons";
	ofLog(OF_LOG_NOTICE, msg);

	// setup force feedback
	setupFF();
}

ofxGamepadLinux::~ofxGamepadLinux() {
}

void ofxGamepadLinux::update() {
	struct js_event event;
	while (read(fd, &event, sizeof(struct js_event)) > 0) {

	    /* see what to do with the event */
	    switch (event.type & ~JS_EVENT_INIT) {
	      case JS_EVENT_AXIS:
		axisChanged(event.number, event.value);
		break;
	      case JS_EVENT_BUTTON:
		buttonChanged(event.number, event.value);
		break;
	    }
	}

}
void ofxGamepadLinux::exit() {
	close(fd);
}

void ofxGamepadLinux::setupFF() {

	vector<string> list = ofSplitString(filename, "js");

	string fName;

	if(list.size()>0)
		fName = "/dev/input/event"+list[list.size()-1]; //There is a problem here, thought that the event number is always the same as the js number, but it is not...

	if ((fdEvent = open(fName.c_str(), O_RDWR)) < 0) { // change from O_RDONLY to O_RDWR need in force feedback
		ofLog(OF_LOG_WARNING,"could not open joystick input event "+fName+". This means force feedback is not going to work");
		return;
	}
	fcntl( fdEvent, F_SETFL, O_NONBLOCK );
	/* Query device */
	if (ioctl(fdEvent, EVIOCGBIT(EV_FF, sizeof(unsigned long) * 4), features) == -1) {
		ofLog(OF_LOG_ERROR,"Ioctl query");
	}

	ofLog(OF_LOG_VERBOSE,"Axes query: ");

	if (test_bit(ABS_X, features)) printf("Axis X ");
	if (test_bit(ABS_Y, features)) printf("Axis Y ");
	if (test_bit(ABS_WHEEL, features)) printf("Wheel ");

	ofLog(OF_LOG_VERBOSE,"\nEffects: ");

	if (test_bit(FF_CONSTANT, features)) 	ofLog(OF_LOG_VERBOSE,"Constant ");
	if (test_bit(FF_PERIODIC, features)) 	ofLog(OF_LOG_VERBOSE,"Periodic ");
	if (test_bit(FF_SPRING, features)) 		ofLog(OF_LOG_VERBOSE,"Spring ");
	if (test_bit(FF_FRICTION, features)) 	ofLog(OF_LOG_VERBOSE,"Friction ");
	if (test_bit(FF_RUMBLE, features)) 		ofLog(OF_LOG_VERBOSE,"Rumble ");

	ofLog(OF_LOG_VERBOSE, "\nNumber of simultaneous effects: "+n_effects );

	if (ioctl(fdEvent, EVIOCGEFFECTS, &n_effects) == -1) {
		ofLog(OF_LOG_ERROR,"Ioctl number of effects");
	}

	/* download a periodic sinusoidal effect */
	effects[0].type = FF_PERIODIC;
	effects[0].id = -1;
	effects[0].u.periodic.waveform = FF_SINE;
	effects[0].u.periodic.period = 0.1*0x100;	/* 0.1 second */
	effects[0].u.periodic.magnitude = 0x4000;	/* 0.5 * Maximum magnitude */
	effects[0].u.periodic.offset = 0;
	effects[0].u.periodic.phase = 0;
	effects[0].direction = 0x4000;	/* Along X axis */
	effects[0].u.periodic.envelope.attack_length = 0x100;
	effects[0].u.periodic.envelope.attack_level = 0;
	effects[0].u.periodic.envelope.fade_length = 0x100;
	effects[0].u.periodic.envelope.fade_level = 0;
	effects[0].trigger.button = 0;
	effects[0].trigger.interval = 0;
	effects[0].replay.length = 20000;  /* 20 seconds */
	effects[0].replay.delay = 0;

	if (ioctl(fdEvent, EVIOCSFF, &effects[0]) == -1) {
		ofLog(OF_LOG_ERROR,"Upload effects[0]");
	}

	/* download a constant effect */
	effects[1].type = FF_CONSTANT;
	effects[1].id = -1;
	effects[1].u.constant.level = 0x2000;	/* Strength : 25 % */
	effects[1].direction = 0x6000;	/* 135 degrees */
	effects[1].u.constant.envelope.attack_length = 0x100;
	effects[1].u.constant.envelope.attack_level = 0;
	effects[1].u.constant.envelope.fade_length = 0x100;
	effects[1].u.constant.envelope.fade_level = 0;
	effects[1].trigger.button = 0;
	effects[1].trigger.interval = 0;
	effects[1].replay.length = 20000;  /* 20 seconds */
	effects[1].replay.delay = 0;

	if (ioctl(fdEvent, EVIOCSFF, &effects[1]) == -1) {
		ofLog(OF_LOG_ERROR,"Upload effects[1]");
	}

	/* download an condition spring effect */
	effects[2].type = FF_SPRING;
	effects[2].id = -1;
	effects[2].u.condition[0].right_saturation = 0x7fff;
	effects[2].u.condition[0].left_saturation = 0x7fff;
	effects[2].u.condition[0].right_coeff = 0x2000;
	effects[2].u.condition[0].left_coeff = 0x2000;
	effects[2].u.condition[0].deadband = 0x0;
	effects[2].u.condition[0].center = 0x0;
	effects[2].u.condition[1] = effects[2].u.condition[0];
	effects[2].trigger.button = 0;
	effects[2].trigger.interval = 0;
	effects[2].replay.length = 20000;  /* 20 seconds */
	effects[2].replay.delay = 0;

	if (ioctl(fdEvent, EVIOCSFF, &effects[2]) == -1) {
		ofLog(OF_LOG_ERROR,"Upload effects[2]");
	}

	/* download an condition damper effect */
	effects[3].type = FF_DAMPER;
	effects[3].id = -1;
	effects[3].u.condition[0].right_saturation = 0x7fff;
	effects[3].u.condition[0].left_saturation = 0x7fff;
	effects[3].u.condition[0].right_coeff = 0x2000;
	effects[3].u.condition[0].left_coeff = 0x2000;
	effects[3].u.condition[0].deadband = 0x0;
	effects[3].u.condition[0].center = 0x0;
	effects[3].u.condition[1] = effects[3].u.condition[0];
	effects[3].trigger.button = 0;
	effects[3].trigger.interval = 0;
	effects[3].replay.length = 20000;  /* 20 seconds */
	effects[3].replay.delay = 0;

	if (ioctl(fdEvent, EVIOCSFF, &effects[3]) == -1) {
		ofLog(OF_LOG_ERROR,"Upload effects[3]");
	}

	/* a strong rumbling effect */
	effects[4].type = FF_RUMBLE;
	effects[4].id = -1;
	effects[4].u.rumble.strong_magnitude = 0x8000;
	effects[4].u.rumble.weak_magnitude = 0;
	effects[4].replay.length = 5000;
	effects[4].replay.delay = 1000;

	if (ioctl(fdEvent, EVIOCSFF, &effects[4]) == -1) {
		ofLog(OF_LOG_ERROR,"Upload effects[4]");
	}

	/* a weak rumbling effect */
	effects[5].type = FF_RUMBLE;
	effects[5].id = -1;
	effects[5].u.rumble.strong_magnitude = 0;
	effects[5].u.rumble.weak_magnitude = 0xc000;
	effects[5].replay.length = 5000;
	effects[5].replay.delay = 0;

	if (ioctl(fdEvent, EVIOCSFF, &effects[5]) == -1) {
		ofLog(OF_LOG_ERROR,"Upload effects[5]");
	}
}

void ofxGamepadLinux::playFF(int i) {
	if (i >= 0 && i < N_EFFECTS) {
		play.type = EV_FF;
		play.code = effects[i].id;
		play.value = 1;

		if (write(fdEvent, (const void*) &play, sizeof(play)) == -1) {
			ofLog(OF_LOG_ERROR,"Play effect");
		}
		ofLog(OF_LOG_VERBOSE,"Now Playing: "+string(effect_names[i])+"\n");
	} else {
		ofLog(OF_LOG_ERROR,"There is no such effect");
	}
}

void ofxGamepadLinux::stopAllFF() {
	for (int i=0; i<N_EFFECTS; ++i) {
		stop.type = EV_FF;
		stop.code =  effects[i].id;
		stop.value = 0;

		if (write(fd, (const void*) &stop, sizeof(stop)) == -1) {
			ofLog(OF_LOG_ERROR,"Stop effect"+ofToString(i));
		}
	}
}

void ofxGamepadLinux::rumble(float level) {
	playFF(0);
}
#endif
