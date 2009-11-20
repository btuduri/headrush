#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// Having this struct inside here means it won't make a copy each time you #include it

struct Ball
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
#define 	ACCEL				0.050
#define 	FRICTION			0.045
#define		MAXACCEL			5
#define		BOUNCE_X_DEADEN		1.0125
#define		JUMPSPEED			5
#define		GRAVITY				0.2

	// Defines for player (ball) status
	
#define		NORMAL				0
#define		JUMPING				1
#define		FALLING				2

	//	Defines for the balls type
	
#define		NORMAL				0
#define		PLAYER				1

#endif

// This means that g_playerBall; is defined externally (in Globals.cpp)

extern Ball g_playerBall;