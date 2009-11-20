#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// Having this struct inside here means it won't make a copy each time you #include it

struct Ball							// define the elements that construct our 'balls'
{
	float X;
	float Y;
	float XSpeed;
	float YSpeed;
	float Angle;
	int Status;
	int Type;
};

	// Defines for the physic effects
#define 	ACCEL				0.050			// Horizontal acceleration on forced movement
#define 	FRICTION			0.045			// Speed of slowdown when no force applied
#define		MAXACCEL			5				// Maximum horizontal speed
#define		BOUNCE_X_DEADEN		1.0125			// how much to deaden a horzontal bounce
#define		JUMPSPEED			5				// initial speed of a jump (negetive)
#define		GRAVITY				0.2				// force of gravity
#define		BOUNCEFACTOR		1.75			// used to reverse a vertical drop for a bounce
#define		BOUNCEFACTORAMOUNT	1.55			// the amount of bounce to allow on a vertical drop

	// Defines for player (ball) status
	
#define		NORMAL				0
#define		JUMPING				1
#define		FALLING				2
#define		GROUNDTOUCH			3

	//	Defines for the balls type
	
#define		NORMAL				0
#define		PLAYER				1

#endif

// This means that g_playerBall; is defined externally (in Globals.cpp)

extern Ball g_playerBall;