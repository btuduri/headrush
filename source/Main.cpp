// ------------------------
// HeadRoll Demo
// ------------------------

#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "sprite_ball.h"		// Include header file for sprites
#include "Globals.h"
#include "Control.h"

int main(void)
{

	videoSetMode(MODE_0_2D);		// Set graphics mode
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);	// Only need sprites for now
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, false);	// This just sets up an area in RAM for storing temporary OAM data
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	// These routines allocates memory for the sprites tile data
	u16* gfxRedBall = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	u16* gfxRedBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	
	u16* gfxBlueBall = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	u16* gfxBlueBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);

	// Basic dma tile and palette data
	dmaCopy(sprite_ballTiles, gfxRedBall, 32 * 32 * 2);
	dmaCopy(sprite_ballTiles, gfxRedBallSub, 32 * 32 * 2);
	
	dmaCopy(sprite_ballTiles + 256, gfxBlueBall, 32 * 32 * 2);
	dmaCopy(sprite_ballTiles + 256, gfxBlueBallSub, 32 * 32 * 2);

	dmaCopy(sprite_ballPal, SPRITE_PALETTE, 512);
	dmaCopy(sprite_ballPal, SPRITE_PALETTE_SUB, 512);

	// This is defining playerBall again. So it's using this not the global one. So this will not work
	//Ball playerBall;
	
	g_playerBall.X = 112;
	g_playerBall.Y = 192-32;
	g_playerBall.Angle = 0;
	g_playerBall.Status = NORMAL;

	while(1)
	{
		// controlHead; You forgot the parenthesis here
		controlHead();
		
		moveHead();
		
		// Draw sprites
		oamRotateScale(&oamSub, 0, g_playerBall.Angle, intToFixed(1, 8), intToFixed(1, 8));

		oamSet(&oamSub, 0, g_playerBall.X, g_playerBall.Y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, gfxRedBall, 0, false, false,	false, false, false);
	
		// Wait for vblank
		swiWaitForVBlank();
		
		// Update temp oam to real oam (must be done during a vblank so it's put here just under the vblank wait)
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		
		
	}

	// Will never reach here
	return 0;
}
