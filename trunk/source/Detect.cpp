#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "ColMap.h"
#include "Control.h"

//
// Check the char block at the centre of our ball
//
int feetCentre(float Xcoord,float Ycoord, int Type)
{
	int x = 0;
	int y = 0;
	int ySettle = 0;
	
	if (Type == BALLTYPE_NORMAL)
	{
		x = (int)Xcoord + 12;
		y = (int)Ycoord + 24;
	}
	else if (Type == BALLTYPE_PLAYER)
	{
		x = (int)Xcoord + (int)g_levelX + 12;
		y = (int)Ycoord + (int)g_levelY + 24;
		ySettle = (int)g_levelY;
	}

//	if (((Ycoord/8)*8) - ((y/8)*8) < MAXYSPEED)
	if ( ((int)Ycoord + ySettle & 7) <= MAXYSPEED )
	{
	return bLevelData[((y/8)*64) + (x/8)];
	}
	else return 0;

}

// Calculate if there is a collision between two balls
// Pass in via pointers so values can be changed

// WHY does it make MEGA jumps???
void checkCollision(Ball* pBall1, Ball* pBall2)
{
//	if (((pBall1->X >= pBall2->X) && (pBall1->X <= pBall2->X+BALLSIZE)) && ((pBall1->Y >= pBall2->Y) && (pBall1->Y <= pBall2->Y+BALLSIZE)))
	{
		int xDist = pBall2->X - pBall1->X;
		int yDist = pBall2->Y - pBall1->Y;
		int dist = sqrt(xDist * xDist + yDist * yDist);
		float angle = atan2(yDist, xDist);
	 
		if (dist < BALLSIZE)
		{
			// Uncomment the following two lines so the balls repell each other instead of ball1 pushing ball2 away
			pBall1->X = pBall2->X - (BALLSIZE * cos(angle));
			pBall1->Y = pBall2->Y - (BALLSIZE * sin(angle));
		
			pBall2->X = pBall1->X + (BALLSIZE * cos(angle));
			pBall2->Y = pBall1->Y + (BALLSIZE * sin(angle));

		}
	}
}

// Fix the boundaries of a ball so it doesn't go off the screen
void fixBoundary(Ball* pBall)
{
	if(pBall->X + scrollCheckX(pBall->Type)< 0)
		pBall->X = 0;
	if(pBall->Y + scrollCheckY(pBall->Type)< 0)
		pBall->Y = 0;
//	if(pBall->X > LEVEL_WIDTH - BALLSIZE)
//		pBall->X = LEVEL_WIDTH - BALLSIZE;
//	if(pBall->Y > SCREEN_HEIGHT - BALLSIZE)
//		pBall->Y = SCREEN_HEIGHT - BALLSIZE;
}
