#include <nds.h>
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// Having this struct inside here means it won't make a copy each time you #include it

struct Ball							// define the elements that construct our 'balls'
{
	u16* Gfx;
	float X;
	float Y;
	float XSpeed;
	float YSpeed;
	int Angle;
	int Status;
	int Type;
	int Action;
};

#define BG0_MAP_BASE				26
#define BG0_MAP_BASE_SUB			26

#define BG1_MAP_BASE				27
#define BG1_MAP_BASE_SUB			27

#define BG2_MAP_BASE				28
#define BG2_MAP_BASE_SUB			28

#define BG3_MAP_BASE				30
#define BG3_MAP_BASE_SUB			30

#define BG0_TILE_BASE				7
#define BG0_TILE_BASE_SUB			7

#define BG1_TILE_BASE				5
#define BG1_TILE_BASE_SUB			5

#define BG2_TILE_BASE				3
#define BG2_TILE_BASE_SUB			3

#define BG3_TILE_BASE				0
#define BG3_TILE_BASE_SUB			0
	
	// Defines for the physic effects
#define 	ACCEL				0.050			// Horizontal acceleration on forced movement
#define 	FRICTION			0.045			// Speed of slowdown when no force applied
#define		MAXACCEL			5				// Maximum horizontal speed
#define		BOUNCE_X_DEADEN		1.0125			// how much to deaden a horzontal bounce
#define		JUMPSPEED			5				// initial speed of a jump (negetive)
#define		GRAVITY				0.2				// force of gravity
#define		BOUNCEFACTOR		1.75			// used to reverse a vertical drop for a bounce
#define		BOUNCEFACTORAMOUNT	1.55			// the amount of bounce to allow on a vertical drop

	// Defines for ball properties
#define		BALLSIZE			24				// Size of balls
#define		BALLOFFSET			4				// Ball offset of balls
#define		BALLCOUNT			2				// Number of balls

	// Defines for player (ball) status
	
#define		BALLSTATUS_NORMAL				0
#define		BALLSTATUS_JUMPING				1
#define		BALLSTATUS_FALLING				2
#define		BALLSTATUS_GROUNDTOUCH			3

	//	Defines for the balls type
	
#define		BALLTYPE_NORMAL		0
#define		BALLTYPE_PLAYER		1

	// Defines for ball actions
#define		ACTION_NONE			0
#define		ACTION_MOVELEFT		1
#define		ACTION_MOVERIGHT	2
#define		ACTION_SLOW			3
#define		ACTION_JUMP			4

#endif

// This means that g_playerBall; is defined externally (in Globals.cpp)

extern Ball g_ballArray[BALLCOUNT];