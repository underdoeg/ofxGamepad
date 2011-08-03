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
	ssize_t len = read(fd, &event, sizeof(struct js_event));
	if (len < 0) {
		std::ostringstream str;
		ofLog(OF_LOG_WARNING, "something strange happend while reading joystick data");
	} else if (len == sizeof(event)) {
		if (event.type & JS_EVENT_AXIS) {
			axisChanged(int(event.number), int(event.value));
		} else if (event.type & JS_EVENT_BUTTON) {
			buttonChanged(int(event.number), bool(event.value));
		}
	} else {
		ofLog(OF_LOG_ERROR, "Joystick::update(): unknown read error");
	}
}
void ofxGamepadLinux::exit()
{
	close(fd);
}
