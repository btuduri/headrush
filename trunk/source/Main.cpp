// ------------------------
// HeadRoll Demo
// ------------------------

#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "sprite_ball.h"		// Include header file for sprites
#include "font.h"
#include "background01.h"
#include "level01.h"
#include "Globals.h"
#include "Control.h"
#include "Text.h"

int main(void)
{
	videoSetMode(MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE);
	
	vramSetMainBanks(VRAM_A_MAIN_BG, VRAM_B_MAIN_SPRITE_0x06400000, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);
	
	bgInit(0, BgType_Text4bpp, BgSize_T_256x256, BG0_MAP_BASE, BG0_TILE_BASE);
	bgInit(1, BgType_Text8bpp, BgSize_T_256x256, BG1_MAP_BASE, BG1_TILE_BASE);
	bgInit(2, BgType_Text8bpp, BgSize_T_256x256, BG2_MAP_BASE, BG2_TILE_BASE);
	bgInit(3, BgType_Text8bpp, BgSize_T_256x256, BG3_MAP_BASE, BG3_TILE_BASE);
	
	bgInitSub(0, BgType_Text4bpp, BgSize_T_256x256, BG0_MAP_BASE_SUB, BG0_TILE_BASE_SUB);
	bgInitSub(1, BgType_Text8bpp, BgSize_T_256x256, BG1_MAP_BASE_SUB, BG1_TILE_BASE_SUB);
	bgInitSub(2, BgType_Text8bpp, BgSize_T_256x256, BG2_MAP_BASE_SUB, BG2_TILE_BASE_SUB);
	bgInitSub(3, BgType_Text8bpp, BgSize_T_256x256, BG3_MAP_BASE_SUB, BG3_TILE_BASE_SUB);

	oamInit(&oamMain, SpriteMapping_1D_32, false);	// This just sets up an area in RAM for storing temporary OAM data
	oamInit(&oamSub, SpriteMapping_1D_32, false);
	
	dmaFillHalfWords(0, BG_PALETTE_SUB, 512);
	dmaFillHalfWords(0, BG_PALETTE, 512);
	dmaFillHalfWords(0, BG_MAP_RAM_SUB(BG0_MAP_BASE_SUB), 2048);
	dmaFillHalfWords(0, BG_MAP_RAM(BG0_MAP_BASE), 2048);
	dmaFillHalfWords(0, BG_MAP_RAM_SUB(BG1_MAP_BASE_SUB), 2048);
	dmaFillHalfWords(0, BG_MAP_RAM(BG1_MAP_BASE), 2048); 

	// These routines allocates memory for the sprites tile data
	u16* gfxPlayerBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	u16* gfxEnemyBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);

	// Basic dma tile and palette data
	dmaCopy(sprite_ballTiles, gfxPlayerBallSub, 32 * 32 * 2);
	dmaCopy(sprite_ballTiles + 256, gfxEnemyBallSub, 32 * 32 * 2);

	dmaCopy(sprite_ballPal, SPRITE_PALETTE_SUB, 512);
	
	dmaCopy(fontTiles, BG_TILE_RAM(BG0_TILE_BASE), fontTilesLen);
	dmaCopy(fontTiles, BG_TILE_RAM_SUB(BG0_TILE_BASE_SUB) , fontTilesLen);
	
	dmaCopy(background01Tiles, BG_TILE_RAM(BG1_TILE_BASE), background01TilesLen);
	dmaCopy(level01Tiles, BG_TILE_RAM_SUB(BG1_TILE_BASE_SUB), level01TilesLen);
	dmaCopy(background01Map, BG_MAP_RAM(BG1_MAP_BASE), background01MapLen);
	dmaCopy(level01Map, BG_MAP_RAM_SUB(BG1_MAP_BASE_SUB), level01MapLen);
	dmaCopy(background01Pal, BG_PALETTE, background01PalLen);
	dmaCopy(level01Pal, BG_PALETTE_SUB, level01PalLen);
	
	DrawString("HEADRUSH", 0, 0, true);

	// This is defining playerBall again. So it's using this not the global one. So this will not work
	//Ball playerBall;
	
	g_playerBall.X = 112;
	g_playerBall.Y = 184-BALLSIZE;
	g_playerBall.Angle = 0;
	g_playerBall.Status = BALLSTATUS_NORMAL;
	g_playerBall.Type = BALLTYPE_PLAYER;
	g_playerBall.YSpeed = 0;
	
	int randAction = ACTION_NONE;
	int randActionArray[BALLCOUNT];
	
	for(int i=1; i<BALLCOUNT; i++)
	{
		randActionArray[i] = ACTION_NONE;
		
		g_ballArray[i].X = rand() % (256-BALLSIZE);
		g_ballArray[i].Y = 184-BALLSIZE;
		g_ballArray[i].Type = BALLTYPE_NORMAL;
		g_ballArray[i].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball
		
		// Copy the ball tiles to each ball in the array
		dmaCopy(sprite_ballTiles + 256, g_ballArray[i].Gfx, 32 * 32 * 2);	
	}
		// INIT PLAYER 
		randActionArray[0] = ACTION_NONE;
		g_ballArray[0].X = 112;
		g_ballArray[0].Y = 184-BALLSIZE;
		g_ballArray[0].Type = BALLTYPE_PLAYER;
		g_ballArray[0].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball
		// Copy the ball tiles to each ball in the array
		dmaCopy(sprite_ballTiles, g_ballArray[0].Gfx, 32 * 32 * 2);	

	while(1)
	{
		// controlHead; You forgot the parenthesis here
		controlHead();
		
		if(rand() % 32 == 0) // Only move enemy occationally
		{
			// rand() % 5 returns a random value from 0 to 4
			randAction = rand() % 5;
		}
		
		updateHead(&g_playerBall);		// call updateHead with the address of the struct g_playerBall
		
		fixBoundary(&g_playerBall);		// Fix boundary of player
		
		// Draw sprites
		oamRotateScale(&oamSub, 0, g_playerBall.Angle, intToFixed(1, 8), intToFixed(1, 8));	

		oamSet(&oamSub, 0, g_playerBall.X - BALLOFFSET, g_playerBall.Y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, gfxPlayerBallSub, 0, false, false, false, false, false);
	
		// draw loop
		for(int i=0; i<BALLCOUNT; i++)
		{
			if(rand() % 32 == 0) // Only move enemy occationally
			{
				// rand() % 5 returns a random value from 0 to 4
				randActionArray[i] = rand() % 5;
			}
			
			moveHead(&g_ballArray[i], randActionArray[i]);
			
			updateHead(&g_ballArray[i]);		// call updateHead with the address of the struct
			
			fixBoundary(&g_ballArray[i]);		// Fix boundary 
			
			// Note this only calculates between each ball in the array
			for(int j=0; j<BALLCOUNT; j++)
				checkCollision(&g_ballArray[i], &g_ballArray[j]); // check collision between all balls
			
			fixBoundary(&g_ballArray[i]);		// Fix boundary
			
			oamRotateScale(&oamSub, i + 2, g_ballArray[i].Angle, intToFixed(1, 8), intToFixed(1, 8));
			
			// The second parameter here is the oam id, so each sprite in the array needs to have it's own id
			// The + 2 is jump over the player and enemy balls
			// The 6th parameter from the end is the rotate/scale index. I think we have 32 of them so
			// Again each ball gets it's own 
			oamSet(&oamSub, i + 2, g_ballArray[i].X - BALLOFFSET, g_ballArray[i].Y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, g_ballArray[i].Gfx, i + 2, false, false, false, false, false);
		}
	
		// Wait for vblank
		swiWaitForVBlank();
		
		// Update temp oam to real oam (must be done during a vblank so it's put here just under the vblank wait)
		// oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	// Will never reach here
	return 0;
}
