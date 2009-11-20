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

	if (held & KEY_A)
	{
		if (g_playerBall.Status != JUMPING)
		{
			g_playerBall.Status = JUMPING;
			g_playerBall.YSpeed = -JUMPSPEED;
		}
	}
};


// moveHead move the head! (simple!)
// perhaps?

void moveHead()
{
float playerBallX = g_playerBall.X;

g_playerBall.X = g_playerBall.X + g_playerBall.XSpeed;

if (g_playerBall.X > 256-32 )
	{
		g_playerBall.X = 256-32;
		g_playerBall.XSpeed = -abs(g_playerBall.XSpeed / BOUNCE_X_DEADEN);
	}
else if (g_playerBall.X < 0 )
	{
		g_playerBall.X = 0;
		g_playerBall.XSpeed = abs(g_playerBall.XSpeed / BOUNCE_X_DEADEN);
	}
	
	// Now we need to rotate the head, based on our movement!
	// using rotateHead to pass, initialX, and currentX to return the angle!
	// wow!! It worked!
	g_playerBall.Angle += rotateHead(playerBallX, g_playerBall.X);
	
	// ok, now account for the jump, this needs to check Status and see if we are jumping
	
	if (g_playerBall.Status == JUMPING)
	{
		g_playerBall.Y += g_playerBall.YSpeed;
		
		g_playerBall.YSpeed += GRAVITY;
		
		if (g_playerBall.Y > 192-32)
		{
			if (g_playerBall.YSpeed > 1.75)
			{
				if (g_playerBall.Y > 192-32)
				{
					g_playerBall.Y = 192-32;
					int held = keysHeld();
					if (held & KEY_A)
					{
						g_playerBall.Status = NORMAL;
					}
				}
					
			{
			}
				g_playerBall.YSpeed = -(g_playerBall.YSpeed/1.55);
				g_playerBall.Y = 192-32;
				
			}
			else
			{
				g_playerBall.Y = 192-32;
				g_playerBall.Status = NORMAL;
			}
		}
		
		
		
		
		
	
	
	}
	
};


float rotateHead(float fromX, float toX)	// our rotation function
{

	if (toX < fromX)
	{
		return degreesToAngle((fromX - toX) * 4);
	}
	else if (fromX < toX)
	{
		return -degreesToAngle((toX - fromX) * 4);
	}
	return 0;
}