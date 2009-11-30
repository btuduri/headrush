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
#include "level02.h"
#include "Globals.h"
#include "Control.h"
#include "Text.h"
#include "DrawMap.h"

int main(void)
{
	videoSetMode(MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE);
	
	vramSetMainBanks(VRAM_A_MAIN_BG, VRAM_B_MAIN_SPRITE_0x06400000, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);
	
	bgInit(0, BgType_Text4bpp, BgSize_T_256x256, BG0_MAP_BASE, BG0_TILE_BASE);
	bgInit(1, BgType_Text8bpp, BgSize_T_256x256, BG1_MAP_BASE, BG1_TILE_BASE);
//	bgInit(2, BgType_Text8bpp, BgSize_T_256x256, BG2_MAP_BASE, BG2_TILE_BASE);
//	bgInit(3, BgType_Text8bpp, BgSize_T_256x256, BG3_MAP_BASE, BG3_TILE_BASE);
	
	bgInitSub(0, BgType_Text4bpp, BgSize_T_256x256, BG0_MAP_BASE_SUB, BG0_TILE_BASE_SUB);
	int playBG = bgInitSub(1, BgType_Text8bpp, BgSize_T_512x512, BG1_MAP_BASE_SUB, BG1_TILE_BASE_SUB);
//	bgInitSub(2, BgType_Text8bpp, BgSize_T_256x256, BG2_MAP_BASE_SUB, BG2_TILE_BASE_SUB);
//	bgInitSub(3, BgType_Text8bpp, BgSize_T_256x256, BG3_MAP_BASE_SUB, BG3_TILE_BASE_SUB);

	oamInit(&oamMain, SpriteMapping_1D_32, false);	// This just sets up an area in RAM for storing temporary OAM data
	oamInit(&oamSub, SpriteMapping_1D_32, false);
	
	dmaFillHalfWords(0, BG_PALETTE_SUB, 512);
	dmaFillHalfWords(0, BG_PALETTE, 512);
	dmaFillHalfWords(0, BG_MAP_RAM_SUB(BG0_MAP_BASE_SUB), 2048);
	dmaFillHalfWords(0, BG_MAP_RAM(BG0_MAP_BASE), 2048);
	dmaFillHalfWords(0, BG_MAP_RAM_SUB(BG1_MAP_BASE_SUB), 4096);
	dmaFillHalfWords(0, BG_MAP_RAM(BG1_MAP_BASE), 2048); 

	// These routines allocate memory for the sprites tile data
	u16* gfxPlayerBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	u16* gfxEnemyBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);

	// Basic dma tile and palette data
	dmaCopy(sprite_ballTiles, gfxPlayerBallSub, 32 * 32 * 2);
	dmaCopy(sprite_ballTiles + 256, gfxEnemyBallSub, 32 * 32 * 2);

	dmaCopy(sprite_ballPal, SPRITE_PALETTE_SUB, 512);
	
	dmaCopy(fontTiles, BG_TILE_RAM(BG0_TILE_BASE), fontTilesLen);
	dmaCopy(fontTiles, BG_TILE_RAM_SUB(BG0_TILE_BASE_SUB) , fontTilesLen);
	
	dmaCopy(background01Tiles, BG_TILE_RAM(BG1_TILE_BASE), background01TilesLen);
	dmaCopy(background01Map, BG_MAP_RAM(BG1_MAP_BASE), background01MapLen);
	dmaCopy(background01Pal, BG_PALETTE, background01PalLen);
	
	dmaCopy(level02Tiles, bgGetGfxPtr(playBG), level02TilesLen);
	dmaCopy(level02Map, bgGetMapPtr(playBG), level02MapLen);
	dmaCopy(level02Pal, BG_PALETTE_SUB, level02PalLen);
	
	DrawString("HEADRUSH, whatever next?", 0, 0, false);

	for(int i=1; i<BALLCOUNT; i++)
	{
		g_ballArray[i].Action = ACTION_NONE;	
		g_ballArray[i].X = rand() % (LEVEL_WIDTH-BALLSIZE);
		g_ballArray[i].Y = rand() % (LEVEL_HEIGHT-BALLSIZE);
		g_ballArray[i].Type = BALLTYPE_NORMAL;
		g_ballArray[i].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball	
		// Copy the ball tiles to each ball in the array
		dmaCopy(sprite_ballTiles + 256, g_ballArray[i].Gfx, 32 * 32 * 2);	
	}
		// INIT PLAYER 
		g_ballArray[0].Action = ACTION_NONE;
		g_ballArray[0].X = 112;
		g_ballArray[0].Y = 184-BALLSIZE;
		g_ballArray[0].Type = BALLTYPE_PLAYER;
		g_ballArray[0].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball
		// Copy the ball tiles to each ball in the array
		dmaCopy(sprite_ballTiles, g_ballArray[0].Gfx, 32 * 32 * 2);	

	g_levelX = 0;
	g_levelY = 512-192;

	while(1)
	{
	
		drawMap();
	
		// draw loop
		for(register int i=0; i<BALLCOUNT; i++)
		{
			moveHead(&g_ballArray[i]);
			
			updateHead(&g_ballArray[i]);		// call updateHead with the address of the struct

			// Note this only calculates between each ball in the array
		//	for(int j=0; j<BALLCOUNT; j++)
		//		checkCollision(&g_ballArray[i], &g_ballArray[j]); // check collision between all balls
			
			fixBoundary(&g_ballArray[i]);		// Fix boundary
			
			oamRotateScale(&oamSub, i + 2, g_ballArray[i].Angle, intToFixed(1, 8), intToFixed(1, 8));
			
			int x = g_ballArray[i].X - g_levelX;
			int y = g_ballArray[i].Y - g_levelY;
			
			// The second parameter here is the oam id, so each sprite in the array needs to have it's own id
			// The + 2 is jump over the player and enemy balls
			// The 6th parameter from the end is the rotate/scale index. I think we have 32 of them so
			// Again each ball gets it's own 
			if ((x > -(BALLSIZE + BALLOFFSET) && x < (SCREEN_WIDTH + BALLOFFSET)) && (y > -(BALLSIZE + BALLOFFSET) && y < (SCREEN_HEIGHT + BALLOFFSET)) && (g_ballArray[i].Type == BALLTYPE_NORMAL))
				oamSet(&oamSub, i + 2, x - BALLOFFSET, y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, g_ballArray[i].Gfx, i + 2, false, false, false, false, false);
			else if (g_ballArray[i].Type == BALLTYPE_PLAYER)
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
