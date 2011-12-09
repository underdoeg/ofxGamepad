#ifndef OFX_GAMEPAD_CONFIGS
#define OFX_GAMEPAD_CONFIGS

#define PS3_NAME "Sony PLAYSTATION(R)3 Controller"

enum GAMEPAD_TYPE{
	GAMEPAD_UNKNOWN,
	GAMEPAD_PS3,
	GAMEPAD_XBOX
};

enum PS3_CONFIG {
    PS3_STICK_L_X = 0,
    PS3_STICK_L_Y = 1,
    PS3_STICK_R_X = 2,
    PS3_STICK_R_Y = 3,
	PS3_THRUST_L2 = 12,
	PS3_THRUST_R2 = 13,
    PS3_BTN_CROSS = 14,
    PS3_BTN_CIRCLE = 13,
    PS3_BTN_SQUARE = 15,
    PS3_BTN_TRIANGLE = 12,
    PS3_BTN_R1 = 11,
    PS3_BTN_R2 = 9,
    PS3_BTN_L1 = 10,
    PS3_BTN_L2 = 8,
    PS3_BTN_SELECT = 0,
    PS3_BTN_START = 3,
    PS3_BTN_STICK_R = 2,
    PS3_BTN_STICK_L = 1,
    PS3_DIR_RIGHT = 5,
    PS3_DIR_UP = 4,
    PS3_DIR_LEFT = 7,
    PS3_DIR_DOWN = 6
};

#define XBOX_NAME "Microsoft Wireless 360 Controller"

enum XBOX_CONFIG {
    XB_STICK_L_X = 2,
    XB_STICK_L_Y = 3,
    XB_STICK_R_X = 4,
    XB_STICK_R_Y = 5,
    XB_STICK_LT = 0,
    XB_STICK_RT = 1,
    XB_BTN_A = 11,
    XB_BTN_B = 12,
    XB_BTN_X = 13,
    XB_BTN_Y = 14,
    XB_BTN_BACK = 5,
    XB_BTN_START = 4,
    XB_BTN_LOGO = 10,
    XB_BTN_LB = 8,
    XB_BTN_RB = 9,
    XB_BTN_STICK_L = 6,
    XB_BTN_STICK_R = 7,
    XB_DIR_UP = 0,
    XB_DIR_DOWN = 1,
    XB_DIR_LEFT = 2,
    XB_DIR_RIGHT = 3
};

#endif
