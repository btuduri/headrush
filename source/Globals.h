#include <nds.h>
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// Having this struct inside here means it won't make a copy each time you #include it

struct Sprite							// define the elements that construct our 'balls'
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

#define BG1_MAP_BASE				31
#define BG1_MAP_BASE_SUB			20

#define BG2_MAP_BASE				28
#define BG2_MAP_BASE_SUB			24

#define BG3_MAP_BASE				30
#define BG3_MAP_BASE_SUB			30

#define BG0_TILE_BASE				7
#define BG0_TILE_BASE_SUB			7

#define BG1_TILE_BASE				5
#define BG1_TILE_BASE_SUB			0

#define BG2_TILE_BASE				3
#define BG2_TILE_BASE_SUB			4

#define BG3_TILE_BASE				0
#define BG3_TILE_BASE_SUB			2

#define SCREEN_WIDTH				256
#define SCREEN_HEIGHT				192

#define LEVEL_WIDTH					512
#define LEVEL_HEIGHT				512
	
	// Defines for the physic effects
#define 	ACCEL				0.050			// Horizontal acceleration on forced movement		( *1.5 when turning)
#define 	FRICTION			0.035			// Speed of slowdown when no force applied
#define		MAXACCEL			4.5				// Maximum horizontal speed
#define		BOUNCE_X_DEADEN		1.015			// how much to deaden a horzontal bounce
#define		JUMPSPEED			3.95			// initial speed of a jump (negetive)				( > higher jump)
#define		GRAVITY				0.195			// force of gravity									(affects jump and bounce)
#define		BOUNCEFACTOR		1.25			// used to reverse a vertical drop for a bounce		( < for more bounces)
#define		BOUNCEFACTORAMOUNT	1.55			// the amount of bounce to allow on a vertical drop	( > for smaller bounce)
#define		MAXYSPEED			5				// maximum speed you can drop

	// Defines for ball properties
#define		BALLSCROLLX			64				// closeness to sides to enable scrolling
#define		BALLSCROLLY			64				// closeness to top/bot to enable scrolling
#define		BALLSIZE			24				// Size of balls
#define		BALLOFFSET			4				// Ball offset of balls
#define		BALLCOUNT			24				// Number of balls

	// Defines for player (ball) status
	
#define		BALLSTATUS_NORMAL				0
#define		BALLSTATUS_JUMPING				1
#define		BALLSTATUS_FALLING				2
#define		BALLSTATUS_GROUNDTOUCH			3

	//	Defines for the balls type
	
#define		BALLTYPE_NULL		0
#define		BALLTYPE_PLAYER		1
#define		BALLTYPE_EVILBALL	2

	// Defines for ball actions
#define		ACTION_NONE			0
#define		ACTION_MOVELEFT		1
#define		ACTION_MOVERIGHT	2
#define		ACTION_SLOW			3
#define		ACTION_JUMP			4

#endif

// This means that g_playerBall; is defined externally (in Globals.cpp)

extern Sprite g_spriteArray[BALLCOUNT];
extern int g_reJump;
extern float g_levelX;
extern float g_levelY;