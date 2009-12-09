// ------------------------
// HeadRoll Demo
// ------------------------

#include <nds.h>				// Main nds header (includes other ds headers)
#include <math.h>				// For the sqrt, atan2, sin, cos etc.
#include <stdio.h>				// For outputting to the console
#include <stdlib.h>				// Define's some standard "C" functions
#include <unistd.h>

#include "Box2D.h"
#include "sprite_ball.h"		// Include header file for sprites
#include "sprite_col.h"
#include "font.h"
#include "logo.h"
#include "background01.h"
#include "back01.h"
#include "level02.h"
#include "level03.h"
#include "Globals.h"
#include "Control.h"
#include "Text.h"
#include "DrawMap.h"

void initBox2D()
{
	g_worldAABB = new b2AABB();
	g_worldAABB->minVertex.Set(-100.0f, -100.0f);
	g_worldAABB->maxVertex.Set(100.0f, 100.0f);
	
	g_gravity = new b2Vec2(0.0f, -10.0f);
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(50.0f, 10.0f);
	g_groundBoxDef->density = 0.0f;

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -10.0f);
	g_groundBodyDef->AddShape(g_groundBoxDef);

	g_world->CreateBody(g_groundBodyDef);

	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(1.0f, 1.0f);
	g_boxDef->density = 0.01f;
	g_boxDef->friction = 0.1f;
	
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(0.5f, 4.0f);
	g_bodyDef->AddShape(g_boxDef);
	
	g_body = g_world->CreateBody(g_bodyDef);

	float timeStep = 1.0f / 60.0f;
	int iterations = 10;
	static char buf[256];

	/*for(int i=0; i<60; ++i)
	{
		g_world->Step(timeStep, iterations);
		b2Vec2 position = g_body->GetOriginPosition();
		float rotation = g_body->GetRotation();
		
		sprintf(buf, "%4.2f %4.2f %4.2f", (float) position.x, (float) position.y, rotation);
		fprintf(stderr, buf);
	}
*/
}

int main(void)
{
	consoleDebugInit(DebugDevice_NOCASH);

	videoSetMode(MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_0_2D | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D_LAYOUT | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE);
	
	vramSetMainBanks(VRAM_A_MAIN_BG, VRAM_B_MAIN_SPRITE_0x06400000, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);
	
	bgInit(0, BgType_Text4bpp, BgSize_T_256x256, BG0_MAP_BASE, BG0_TILE_BASE);
	bgInit(1, BgType_Text8bpp, BgSize_T_256x256, BG1_MAP_BASE, BG1_TILE_BASE);
//	bgInit(2, BgType_Text8bpp, BgSize_T_256x256, BG2_MAP_BASE, BG2_TILE_BASE);
//	bgInit(3, BgType_Text8bpp, BgSize_T_256x256, BG3_MAP_BASE, BG3_TILE_BASE);
	
	bgInitSub(0, BgType_Text4bpp, BgSize_T_256x256, BG0_MAP_BASE_SUB, BG0_TILE_BASE_SUB);
	int playFG = bgInitSub(1, BgType_Text8bpp, BgSize_T_512x512, BG1_MAP_BASE_SUB, BG1_TILE_BASE_SUB);
	int playBG = bgInitSub(2, BgType_Text8bpp, BgSize_T_256x256, BG2_MAP_BASE_SUB, BG2_TILE_BASE_SUB);
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
	dmaCopy(sprite_ballTiles + 512, gfxEnemyBallSub, 32 * 32 * 2);

	dmaCopy(sprite_ballPal, SPRITE_PALETTE_SUB, 512);
	
	dmaCopy(fontTiles, BG_TILE_RAM(BG0_TILE_BASE), fontTilesLen);
//	dmaCopy(fontTiles, BG_TILE_RAg_SUB(BG0_TILE_BASE_SUB) , fontTilesLen);
	
	dmaCopy(logoTiles, BG_TILE_RAM(BG1_TILE_BASE), logoTilesLen);
	dmaCopy(logoMap, BG_MAP_RAM(BG1_MAP_BASE), logoMapLen);
	dmaCopy(logoPal, BG_PALETTE, logoPalLen);
	
	dmaCopy(level03Tiles, bgGetGfxPtr(playFG), level03TilesLen);
	dmaCopy(level03Map, bgGetMapPtr(playFG), level03MapLen);
	dmaCopy(level03Pal, BG_PALETTE_SUB, level03PalLen);
	
	dmaCopy(back01Tiles, bgGetGfxPtr(playBG), back01TilesLen);
	dmaCopy(back01Map, bgGetMapPtr(playBG), back01MapLen);
	
	DrawString("HEADRUSH, whatever next?", 0, 0, false);
	DrawString("Stupid fucking game idea!", 0, 22, false);
	DrawString("ALPHA 0.00000000004 and a chuff", 0, 2, false);
	
	// --------------------------
	// These are "collision" sprites to help us located rounding issues
	
	g_colSprite1.Gfx = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
	g_colSprite2.Gfx = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
	
	dmaCopy(sprite_colTiles, g_colSprite1.Gfx, 8 * 8 * 2);
	dmaCopy(sprite_colTiles + 16, g_colSprite2.Gfx, 8 * 8 * 2);
	//g_colSprite1.X = 128;
	//g_colSprite1.Y = 128;
	
	initBox2D();
	static char buf[256];
	float timeStep = 1.0f / 60.0f;
	int iterations = 10;
	
	// --------------------------

	for(int i=1; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = 40; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_spriteArray[i].Y = 32; //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_spriteArray[i].Type = BALLTYPE_EVILBALL;
		g_spriteArray[i].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball	
		// Copy the ball tiles to each ball in the array
		dmaCopy(sprite_ballTiles + 256, g_spriteArray[i].Gfx, 32 * 32 * 2);	
	}
		// INIT PLAYER 
		g_spriteArray[0].Action = ACTION_NONE;
		g_spriteArray[0].X = 112;							// Will need to work out a way to centre the scroll
		g_spriteArray[0].Y = 150-BALLSIZE;				// based on the players initial x/y coord
		g_spriteArray[0].Type = BALLTYPE_PLAYER;
		g_spriteArray[0].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball
		// Copy the ball tiles to each ball in the array
		dmaCopy(sprite_ballTiles, g_spriteArray[0].Gfx, 32 * 32 * 2);	

	g_levelX = 0;
	g_levelY = 0;
	drawMap();


//	char buffer[20];
	while(1)
	{
/*
	sprintf(buffer, "%d X ",(int)g_spriteArray[0].X) ;
	DrawString(buffer, 10, 21, false);
	sprintf(buffer, "%d Y ",(int)g_spriteArray[0].Y + (int) g_levelY) ;
	DrawString(buffer, 16, 21, false);	
	sprintf(buffer, "%d A ",(int)g_spriteArray[0].Action) ;
	DrawString(buffer, 10, 23, false);	
	sprintf(buffer, "%d S ",(int)g_spriteArray[0].Status) ;
	DrawString(buffer, 16, 23, false);
	sprintf(buffer, "%d X OFF ",(((int)g_spriteArray[0].X) + (int) g_levelX) & 7) ;
	DrawString(buffer, 16, 19, false);
	sprintf(buffer, "%d Y OFF ",(((int)g_spriteArray[0].Y) + (int) g_levelY) & 7) ;
	DrawString(buffer, 25, 19, false);
	
	
	sprintf(buffer, "%d X SCRL",(int) g_levelX) ;
	DrawString(buffer, 0, 4, false);	
*/
	drawMap();
	

b2Vec2 posBall = g_body->GetOriginPosition();
g_spriteArray[0].Y = (posBall.y * 50);
g_spriteArray[0].X = posBall.x * 256;

oamSet(&oamSub, 2, g_spriteArray[0].X - BALLOFFSET, g_spriteArray[0].Y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, g_spriteArray[0].Gfx, 2, false, false, false, false, false);
		// draw loop
/*		for(register int i=0; i<BALLCOUNT; i++)
		{
			moveSprite(&g_spriteArray[i]);		// move all active sprites
	
			oamRotateScale(&oamSub, i + 2, g_spriteArray[i].Angle, intToFixed(1, 8), intToFixed(1, 8));	// rotate the sprite
		
			// set a local x/y so we know if we can display the current sprite
	
			int x = g_spriteArray[i].X - g_levelX - BALLOFFSET;
			int y = g_spriteArray[i].Y - g_levelY - BALLOFFSET;
			
			// The second parameter here is the oam id, so each sprite in the array needs to have it's own id
			// The + 2 is jump over the player and enemy balls
			// The 6th parameter from the end is the rotate/scale index. I think we have 32 of them so
			// Again each ball gets it's own 
			if ((x > -BALLSIZE - BALLOFFSET - 4 && x < SCREEN_WIDTH) && (y > -BALLSIZE - BALLOFFSET - 4 && y < SCREEN_HEIGHT) && (g_spriteArray[i].Type == BALLTYPE_EVILBALL))
				oamSet(&oamSub, i + 2, x, y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, g_spriteArray[i].Gfx, i + 2, false, false, false, false, false);
			else if (g_spriteArray[i].Type == BALLTYPE_PLAYER)
				oamSet(&oamSub, i + 2, g_spriteArray[i].X - BALLOFFSET, g_spriteArray[i].Y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, g_spriteArray[i].Gfx, i + 2, false, false, false, false, false);
		}
*/



		g_world->Step(timeStep, iterations);
		b2Vec2 position = g_body->GetOriginPosition();
		float rotation = g_body->GetRotation();
		
		sprintf(buf, "%4.2f %4.2f %4.2f", (float) position.x, (float) position.y, rotation);
		fprintf(stderr, buf);

		
		oamSet(&oamSub, BALLCOUNT + 2, g_colSprite1.X, g_colSprite1.Y, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color, g_colSprite1.Gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, BALLCOUNT + 3, g_colSprite2.X, g_colSprite2.Y, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color, g_colSprite2.Gfx, -1, false, false, false, false, false);
	
		// Wait for vblank
		swiWaitForVBlank();
		
		// Update temp oam to real oam (must be done during a vblank so it's put here just under the vblank wait)
		// oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	// Will never reach here
	return 0;
}
