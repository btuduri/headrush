#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "Control.h"

void controlHead()					// *TO DO* make this work for all balls (even Normals)
{
	scanKeys();						// Read button data
	
	int held = keysHeld();			// Used to calculate if a button is down

	if (held & KEY_L || held & KEY_LEFT) 
	{
		moveHead(&g_playerBall, ACTION_MOVELEFT);
	}
	else if (held & KEY_R || held & KEY_RIGHT)
	{
		moveHead(&g_playerBall, ACTION_MOVERIGHT);
	}
	else
	{
		moveHead(&g_playerBall, ACTION_SLOW);
	}
	// now we need to check for a jump and init is possible
	// check players status for the setting of jump, and if true, donot init untill false

	if (held & KEY_A)
	{
		moveHead(&g_playerBall, ACTION_JUMP);
	}
};

void moveHead(Ball *pBall, int action)
{
	switch(action)
	{
	case ACTION_MOVELEFT:
		pBall->XSpeed = pBall->XSpeed - ACCEL;
		
		if (pBall->XSpeed < -MAXACCEL)
		{
			pBall->XSpeed = -MAXACCEL;
		}
		break;
	case ACTION_MOVERIGHT:
		pBall->XSpeed = pBall->XSpeed + ACCEL;
		
		if (pBall->XSpeed > MAXACCEL)
		{
			pBall->XSpeed = MAXACCEL;
		}	
		break;
	case ACTION_SLOW:
		if (pBall->XSpeed < 0)
		{
			pBall->XSpeed = g_playerBall.XSpeed + FRICTION;
			
			if (pBall->XSpeed > 0)
			{
				pBall->XSpeed = 0;
			}
		}
		else if (pBall->XSpeed > 0)
		{
			pBall->XSpeed = pBall->XSpeed - FRICTION;
			
			if (pBall->XSpeed < 0)
			{
				pBall->XSpeed = 0;
			}
		}
		break;
	case ACTION_JUMP:
		if (pBall->Status != BALLSTATUS_JUMPING && pBall->Status != BALLSTATUS_FALLING)
		{
			pBall->Status = BALLSTATUS_JUMPING;
			pBall->YSpeed = -JUMPSPEED;
		}
		break;
	}
}

// updateHead the head! (simple!)
// perhaps?

void updateHead(Ball* pBall)
{
	float oldBallX = pBall->X;

	pBall->X = pBall->X + pBall->XSpeed;

	if (pBall->X > 256-BALLSIZE)
	{
		pBall->X = 256-BALLSIZE;
		pBall->XSpeed = -abs(pBall->XSpeed / BOUNCE_X_DEADEN);
	}
	else if (pBall->X < 0)
	{
		pBall->X = 0;
		pBall->XSpeed = abs(pBall->XSpeed / BOUNCE_X_DEADEN);
	}
	
	// ok, now account for the jump, this needs to check Status and see if we are jumping
	// but... It only really needs to work with upward movement!
	// we should let detection handle falling, ie. if there is no floor, accelerate down until there is
	//
	
	if ((pBall->Status == BALLSTATUS_JUMPING || pBall->Status == BALLSTATUS_GROUNDTOUCH) && (pBall->YSpeed < 0)) // WHY!! do i have to use double ==
	{
		pBall->Y += pBall->YSpeed;
		
		pBall->YSpeed += GRAVITY;
		
		if ( pBall->YSpeed > 0)
		{
			pBall->Status = BALLSTATUS_FALLING;
		}
		else
		{
			pBall->Status = BALLSTATUS_JUMPING;
		}
	}
	// ok, now we need to check if there is ground below the ball (for now if Y<192-32)
	// and if so, set status to falling and fall (he says with such confidence!!)
	
	else	// only need to worry about this if status is not JUMPING
	
	{
		if (pBall->Y < 184-BALLSIZE)	// this would be replaced with a check for floor!
		{
			pBall->YSpeed += GRAVITY;
			pBall->Y += pBall->YSpeed;
			pBall->Status = BALLSTATUS_FALLING;
			
			if ((pBall->Y > 184-BALLSIZE) && (pBall->YSpeed < BOUNCEFACTOR))
			{
				pBall->Y = 184-BALLSIZE;
				pBall->YSpeed = 0;
				pBall->Status = BALLSTATUS_NORMAL;
			}
			else if ((pBall->Y > 184-BALLSIZE) && (pBall->YSpeed > BOUNCEFACTOR))
			{
				pBall->Y = 184-BALLSIZE;
				pBall->YSpeed = -(pBall->YSpeed / BOUNCEFACTORAMOUNT);
				pBall->Status = BALLSTATUS_GROUNDTOUCH;
			}		
		}
	}
	
	// Now we need to rotate the head, based on our movement!
	// using rotateHead to pass, initialX, and currentX to return the angle!
	// wow!! It worked!
	
	pBall->Angle += rotateHead(oldBallX, pBall->X);
	
};

//
// Calculate rotation based on horizontal movement
//
float rotateHead(float originalX, float currentX)	// our rotation function
{

	if (currentX < originalX)
	{
		return degreesToAngle((originalX - currentX) * 4);
	}
	else if (originalX < currentX)
	{
		return -degreesToAngle((currentX - originalX) * 4);
	}
	
	return 0;
}

// Calculate if there is a collision between two balls
// Pass in via pointers so values can be changed
void checkCollision(Ball* pBall1, Ball* pBall2)
{
	float xDist = pBall2->X - pBall1->X;
	float yDist = pBall2->Y - pBall1->Y;
	float dist = sqrt(xDist * xDist + yDist * yDist);
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

// Fix the boundaries of a ball so it doesn't go off the screen
void fixBoundary(Ball* pBall)
{
	if(pBall->X < 0)
		pBall->X = 0;
	if(pBall->Y < 0)
		pBall->Y = 0;
	if(pBall->X > 256 - BALLSIZE)
		pBall->X = 256 - BALLSIZE;
	if(pBall->Y > 184 - BALLSIZE)
		pBall->Y = 184 - BALLSIZE;
}