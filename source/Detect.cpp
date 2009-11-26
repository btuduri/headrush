#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Globals.h"
#include "ColMap.h"

int feetCentre(float Xcoord,float Ycoord)
{
	int x = Xcoord;
	int y = Ycoord;
	
	if ( ((Ycoord/8)*8) - ((y/8)*8) < MAXYSPEED)
	{
	return bLevelData[(((y+24)/8)*32) + ((x+12)/8)];
	}
	else return 0;

}