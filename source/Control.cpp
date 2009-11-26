#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "Control.h"
#include "Detect.h"

void moveHead(Ball *pBall)

// add control head to this first, only if type == player.
// if type == normal (enemy) then check and set random movement
// we do not need to pass action? Should we add action to the class?

// The problem with this is if we want to move LR and jump, we can't


{
	if (pBall->Type == BALLTYPE_PLAYER)								// = Player control
	{
		scanKeys();						// Read button data
		int held = keysHeld();			// Used to calculate if a button is down
		
		if (held & KEY_L || held & KEY_LEFT)
		{
			pBall->Action = ACTION_MOVELEFT;
		}
		else if (held & KEY_R || held & KEY_RIGHT)
		{
			pBall->Action = ACTION_MOVERIGHT;
		}
		else
		{
		pBall->Action = ACTION_SLOW;
		}
		// now we need to check for a jump and init is possible
		// check players status for the setting of jump, and if true, donot init untill false
		if (held & KEY_A) // || (g_reJump == 1))
		{
		//	pBall->Action = ACTION_JUMP;
			if ((pBall->Status != BALLSTATUS_JUMPING) && (pBall->Status != BALLSTATUS_FALLING))
			{
				pBall->Status = BALLSTATUS_JUMPING;
				pBall->YSpeed = -JUMPSPEED;
				g_reJump = 0;

			}
		}
	}
	else if (pBall->Type == BALLTYPE_NORMAL)							// = Random control
	{
		if(rand() % 32 == 0) // Only move enemy occasionally
		{
			// rand() % 5 returns a random value from 0 to 4
			pBall->Action = rand() % 5;
			if ((pBall->Action == ACTION_JUMP) && (pBall->Status != BALLSTATUS_JUMPING && pBall->Status != BALLSTATUS_FALLING))
			{
				pBall->Status = BALLSTATUS_JUMPING;
				pBall->YSpeed = -JUMPSPEED;		
			}
		}
	}
	
	// Act on the 'action' of the ball
	
	switch(pBall->Action)
	{
	case ACTION_MOVELEFT:
		if ((pBall->Status == BALLSTATUS_NORMAL) && (pBall->XSpeed > 0))		// if we are on the ground
			pBall->XSpeed = pBall->XSpeed - (ACCEL * 2);						// allow a quicker turn
		else
			pBall->XSpeed = pBall->XSpeed - ACCEL;
		if (pBall->XSpeed < -MAXACCEL) pBall->XSpeed = -MAXACCEL;
		break;
	case ACTION_MOVERIGHT:
		if ((pBall->Status == BALLSTATUS_NORMAL) && (pBall->XSpeed < 0))
			pBall->XSpeed = pBall->XSpeed + (ACCEL * 2);
		else
			pBall->XSpeed = pBall->XSpeed + ACCEL;
		if (pBall->XSpeed > MAXACCEL) pBall->XSpeed = MAXACCEL;
		break;
	case ACTION_SLOW:
		if (pBall->XSpeed < 0)
		{
			pBall->XSpeed = pBall->XSpeed + FRICTION;
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
	}
}


// updateHead the Ball
// Here we update the ball based on YSpeed and XSpeed

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
	
// ok, now account for the jump, this needs to check Status and see if we are jumping (or bouncing)
// but... It only really needs to work with upward movement!
	
	if ((pBall->Status == BALLSTATUS_JUMPING || pBall->Status == BALLSTATUS_GROUNDTOUCH) && pBall->YSpeed < 0)
	{
		// jumping up!
		pBall->Y += pBall->YSpeed;	// y speed will be negative
		
		pBall->YSpeed += GRAVITY;	// check if Y speed is now possitive
		
		if ( pBall->YSpeed >= 0)
		{	// Y speed is +
			pBall->Status = BALLSTATUS_FALLING;
		}
		else
		{	// Y speed is -
			pBall->Status = BALLSTATUS_JUMPING;
		}
	}

// ok, now we need to check if there is ground below the ball
// use 'feetCentre' to check the centre of a ball, the value is returned!
	
	else	// we already know that we are not jumping!
	{
		if (feetCentre(pBall->X, pBall->Y) == 0)			// not on the floor
		{	// We are falling (ie. not on the floor)
			if (pBall->YSpeed < MAXYSPEED)
				pBall->YSpeed += GRAVITY;
	
			pBall->Y += pBall->YSpeed;
			pBall->Status = BALLSTATUS_FALLING;
			
			if ((feetCentre(pBall->X, pBall->Y) != 0) && (pBall->YSpeed < BOUNCEFACTOR))
			{	// We have hit the floor and need to stop bouncing.
				pBall->YSpeed = 0;
				pBall->Status = BALLSTATUS_NORMAL;
			}			
			else if ((feetCentre(pBall->X, pBall->Y) != 0) && (pBall->YSpeed > BOUNCEFACTOR))
			{	// We have hit the floor and still have some bounce in us
				pBall->YSpeed = -(pBall->YSpeed / BOUNCEFACTORAMOUNT);
				pBall->Status = BALLSTATUS_GROUNDTOUCH;
			}		
		}
		else												// we are on the floor
		{
			g_reJump = 0;
			pBall->Y = (int(pBall->Y / 8) *8);
			pBall->Status = BALLSTATUS_NORMAL;
		}
		if (keysHeld() & KEY_A)
			g_reJump = 1;
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
	if(pBall->X < 0)
		pBall->X = 0;
	if(pBall->Y < 0)
		pBall->Y = 0;
	if(pBall->X > SCREEN_WIDTH - BALLSIZE)
		pBall->X = SCREEN_WIDTH - BALLSIZE;
	if(pBall->Y > SCREEN_HEIGHT - BALLSIZE)
		pBall->Y = SCREEN_HEIGHT - BALLSIZE;
}
