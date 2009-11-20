#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "Control.h"

void controlHead()
{
	scanKeys();						// Read button data
	
	int held = keysHeld();			// Used to calculate if a button is down

	if (held & KEY_L || held & KEY_LEFT) 
	{
		g_playerBall.XSpeed = g_playerBall.XSpeed - ACCEL;
		if (g_playerBall.XSpeed < -MAXACCEL)
		{
			g_playerBall.XSpeed = -MAXACCEL;
		}
		
	}
	else if (held & KEY_R || held & KEY_RIGHT)
	{
		g_playerBall.XSpeed = g_playerBall.XSpeed + ACCEL;
		if (g_playerBall.XSpeed > MAXACCEL)
		{
			g_playerBall.XSpeed = MAXACCEL;
		}	
	}
	else
	{
		if (g_playerBall.XSpeed < 0)
		{
			g_playerBall.XSpeed = g_playerBall.XSpeed + FRICTION;
			if (g_playerBall.XSpeed > 0)
			{
				g_playerBall.XSpeed = 0;
			}
		}
		else if (g_playerBall.XSpeed > 0)
		{
			g_playerBall.XSpeed = g_playerBall.XSpeed - FRICTION;
			if (g_playerBall.XSpeed < 0)
			{
				g_playerBall.XSpeed = 0;
			}
		}
	}
	// now we need to check for a jump and init is possible
	// check players status for the setting of jump, and if true, donot init untill false

	if ((held & KEY_A) && (g_playerBall.Status != JUMPING && g_playerBall.Status != FALLING))
	{
		g_playerBall.Status = JUMPING;
		g_playerBall.YSpeed = -JUMPSPEED;
	}
};


// moveHead move the head! (simple!)
// perhaps?

void moveHead(Ball* pBall)
{
float oldBallX = pBall->X;

pBall->X = pBall->X + pBall->XSpeed;

if (pBall->X > 256-32 )
	{
		pBall->X = 256-32;
		pBall->XSpeed = -abs(pBall->XSpeed / BOUNCE_X_DEADEN);
	}
else if (pBall->X < 0 )
	{
		pBall->X = 0;
		pBall->XSpeed = abs(pBall->XSpeed / BOUNCE_X_DEADEN);
	}
	
	// ok, now account for the jump, this needs to check Status and see if we are jumping
	// but... It only really needs to work with upward movement!
	// we should let detection handle falling, ie. if there is no floor, accelerate down until there is
	//
	
	if ((pBall->Status == JUMPING) && (pBall->YSpeed < 0)) // WHY!! do i have to use double ==
	{
		pBall->Y += pBall->YSpeed;
		
		pBall->YSpeed += GRAVITY;
		
		if ( pBall->YSpeed > 0)
		{
		pBall->Status = FALLING;
		}
	}
	// ok, now we need to check if there is ground below the ball (for now if Y<192-32)
	// and if so, set status to falling and fall (he says with such confidence!!)
	
	else			// only need to worry about this if status is not JUMPING
	
	{
		if (pBall->Y < 192-32)	// this would be replaced with a check for floor!
		{
			pBall->YSpeed += GRAVITY;
			pBall->Y += pBall->YSpeed;
			pBall->Status = FALLING;
			
			if ((pBall->Y > 192-32) && (pBall->YSpeed < 1.75))
			{
				pBall->Y = 192-32;
				pBall->YSpeed = 0;
				pBall->Status = NORMAL;
			}
			else if ((pBall->Y > 192-32) && (pBall->YSpeed > 1.75))
			{
				pBall->Y = 192-32;
				pBall->YSpeed = -(pBall->YSpeed/1.55);
				pBall->Status = JUMPING;
			}		
		}
	}
	
	// Now we need to rotate the head, based on our movement!
	// using rotateHead to pass, initialX, and currentX to return the angle!
	// wow!! It worked!
	
	pBall->Angle += rotateHead(oldBallX, pBall->X);
	
};


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