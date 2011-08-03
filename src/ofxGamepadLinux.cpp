#include "ofxGamepadLinux.h"
#include <fcntl.h>
#include <linux/joystick.h>

ofxGamepadLinux::ofxGamepadLinux(string f):ofxGamepad()
{
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
}

ofxGamepadLinux::~ofxGamepadLinux()
{
}

void ofxGamepadLinux::update()
{
	struct js_event event;
	read(fd, &event, sizeof(struct js_event));

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
void ofxGamepadLinux::exit()
{
	close(fd);
}
