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
	u16* gfxPlayerBall = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	u16* gfxPlayerBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	
	u16* gfxEnemyBall = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	u16* gfxEnemyBallSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);

	// Basic dma tile and palette data
	dmaCopy(sprite_ballTiles, gfxPlayerBall, 32 * 32 * 2);
	dmaCopy(sprite_ballTiles, gfxPlayerBallSub, 32 * 32 * 2);
	
	dmaCopy(sprite_ballTiles + 256, gfxEnemyBall, 32 * 32 * 2);
	dmaCopy(sprite_ballTiles + 256, gfxEnemyBallSub, 32 * 32 * 2);

	dmaCopy(sprite_ballPal, SPRITE_PALETTE, 512);
	dmaCopy(sprite_ballPal, SPRITE_PALETTE_SUB, 512);
	
	dmaCopy(fontTiles, BG_TILE_RAM(BG0_TILE_BASE), fontTilesLen);
	dmaCopy(fontTiles, BG_TILE_RAM_SUB(BG0_TILE_BASE_SUB) , fontTilesLen);
	
	dmaCopy(background01Tiles, BG_TILE_RAM(BG1_TILE_BASE), background01TilesLen);
	dmaCopy(background01Tiles, BG_TILE_RAM_SUB(BG1_TILE_BASE_SUB), background01TilesLen);
	dmaCopy(background01Map, BG_MAP_RAM(BG1_MAP_BASE), background01MapLen);
	dmaCopy(background01Map, BG_MAP_RAM_SUB(BG1_MAP_BASE_SUB), background01MapLen);
	dmaCopy(background01Pal, BG_PALETTE, background01PalLen);
	dmaCopy(background01Pal, BG_PALETTE_SUB, background01PalLen);
	
	DrawString("HEADRUSH", 0, 0, true);

	// This is defining playerBall again. So it's using this not the global one. So this will not work
	//Ball playerBall;
	
	g_playerBall.X = 112;
	g_playerBall.Y = 192-BALLSIZE;
	g_playerBall.Angle = 0;
	g_playerBall.Status = BALLSTATUS_NORMAL;
	g_playerBall.Type = BALLTYPE_PLAYER;
	g_playerBall.YSpeed = 0;
	
	g_enemyBall.X = 0;
	g_enemyBall.Y = 192-BALLSIZE;
	g_enemyBall.Angle = 0;
	g_enemyBall.Status = BALLSTATUS_NORMAL;
	g_enemyBall.Type = BALLTYPE_NORMAL;
	g_enemyBall.YSpeed = 0;
	
	int randAction = ACTION_NONE;

	while(1)
	{
		// controlHead; You forgot the parenthesis here
		controlHead();
		
		if(rand() % 32 == 0) // Only move enemy occationally
		{
			// Make the enemy move randomly. rand() % 5 returns a random value from 0 to 3
			randAction = rand() % 5;
		}
		
		moveHead(&g_enemyBall, randAction);
		
		updateHead(&g_playerBall);		// call updateHead with the address of the struct g_playerBall
		updateHead(&g_enemyBall);		// call updateHead with the address of the struct g_enemyBall
		
		fixBoundary(&g_playerBall);		// Fix boundary of player
		fixBoundary(&g_enemyBall);		// Fix boundary of enemy
		
		checkCollision(&g_playerBall, &g_enemyBall); // check collision between two balls
		checkCollision(&g_playerBall, &g_enemyBall); // check collision between two balls
		
		fixBoundary(&g_playerBall);		// Fix boundary of player
		fixBoundary(&g_enemyBall);		// Fix boundary of enemy
		
		// Draw sprites
		oamRotateScale(&oamSub, 0, g_playerBall.Angle, intToFixed(1, 8), intToFixed(1, 8));	
		oamRotateScale(&oamSub, 1, g_enemyBall.Angle, intToFixed(1, 8), intToFixed(1, 8));

		oamSet(&oamSub, 0, g_playerBall.X - BALLOFFSET, g_playerBall.Y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, gfxPlayerBall, 0, false, false, false, false, false);
		oamSet(&oamSub, 1, g_enemyBall.X - BALLOFFSET, g_enemyBall.Y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, gfxEnemyBall, 1, false, false, false, false, false);
	
		// Wait for vblank
		swiWaitForVBlank();
		
		// Update temp oam to real oam (must be done during a vblank so it's put here just under the vblank wait)
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	// Will never reach here
	return 0;
}
