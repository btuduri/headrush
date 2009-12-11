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
	g_worldAABB->minVertex.Set(SCREEN_WIDTH * -SCALE, SCREEN_HEIGHT * -SCALE);
	g_worldAABB->maxVertex.Set(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);
	
	g_gravity = new b2Vec2(0.0f, -10.0f);
	
	bool doSleep = true;
	
	g_world = new b2World(*g_worldAABB, *g_gravity, doSleep);
	
	g_groundBoxDef = new b2BoxDef();
	g_groundBoxDef->extents.Set(50.0f, 10.0f);
	g_groundBoxDef->density = 0.0f;

	g_groundBodyDef = new b2BodyDef(); 
	g_groundBodyDef->position.Set(0.0f, -20.0f);
	g_groundBodyDef->AddShape(g_groundBoxDef);

	g_world->CreateBody(g_groundBodyDef);
//
	g_plat = new b2PolyDef();	// HOW THE FUCK?
	g_plat->vertexCount = 6;
	g_plat->vertices[0].Set(0.0f, 0.4f);
	g_plat->vertices[1].Set(-3.0f, 0.05f);
	g_plat->vertices[2].Set(-3.0f, -0.4f);
	g_plat->vertices[3].Set(0.0f, -0.5f);
	g_plat->vertices[4].Set(5.8f, -0.5f);
	g_plat->vertices[5].Set(5.7f, 0.7f);
	g_plat->density = 0.0f;
	g_plat->friction = 2.0f;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(9.5f, 3.4f);
	g_bodyDef->AddShape(g_plat);
	g_world->CreateBody(g_bodyDef);


	g_plat = new b2PolyDef();	// HOW THE FUCK?
	g_plat->vertexCount = 4;
	g_plat->vertices[0].Set(0.0f, 0.5f);
	g_plat->vertices[1].Set(0.0f, -0.5f);
	g_plat->vertices[2].Set(6.0f, -1.0f);
	g_plat->vertices[3].Set(6.0f, 1.5f);
	g_plat->density = 0.0f;
	g_plat->friction = 2.0f;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(20.8f, 3.6f);
	g_bodyDef->AddShape(g_plat);
	g_world->CreateBody(g_bodyDef);


	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(4.7f, 1.0f);
	g_boxDef->density = 0.0f;
	g_boxDef->friction = 1.0f;
	g_boxDef->restitution = 0.0F;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(17.5f, 0.6f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);
	
	
	g_boxDef = new b2BoxDef();
	g_boxDef->extents.Set(1.0f, 20.0f);
	g_boxDef->density = 0.0f;
	g_boxDef->friction = 1.0f;
	g_boxDef->restitution = 0.0F;
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(25.6f, 19.2f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);	
	g_bodyDef = new b2BodyDef();
	g_bodyDef->position.Set(-2.0f, 19.2f);
	g_bodyDef->AddShape(g_boxDef);
	g_world->CreateBody(g_bodyDef);	
	
	
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
	DrawString("WELL!! Looking a bit better?", 0, 22, false);
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
	
	// --------------------------

	for(int i=1; i<BALLCOUNT; i++)
	{
		g_spriteArray[i].Action = ACTION_NONE;	
		g_spriteArray[i].X = (rand() % 100) + 140; //(rand() % (LEVEL_WIDTH-(BALLSIZE * 2))) + BALLSIZE * 2;
		g_spriteArray[i].Y =  192 - ((rand() % 100)); //(rand() % (LEVEL_HEIGHT-(BALLSIZE * 2))) + BALLSIZE;
		g_spriteArray[i].Type = BALLTYPE_EVILBALL;
		g_spriteArray[i].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball	
		dmaCopy(sprite_ballTiles + 512, g_spriteArray[i].Gfx, 32 * 32 * 2);		// Copy the ball tiles to each ball in the array
		
		g_spriteArray[i].CircleDef = new b2CircleDef();
		g_spriteArray[i].BodyDef =  new b2BodyDef();
		
		g_spriteArray[i].CircleDef->radius = 24 / 2 * SCALE; 
		g_spriteArray[i].CircleDef->density = 1.0F; 
		g_spriteArray[i].CircleDef->friction = 0.5F; 
		g_spriteArray[i].CircleDef->restitution = 0.8F; 

		g_spriteArray[i].BodyDef->position.Set(g_spriteArray[i].X * SCALE, g_spriteArray[i].Y * SCALE);
		g_spriteArray[i].BodyDef->AddShape(g_spriteArray[i].CircleDef);
		
		g_spriteArray[i].Body = g_world->CreateBody(g_spriteArray[i].BodyDef);
	}
	
	// INIT PLAYER
		g_spriteArray[0].Action = ACTION_NONE;	
		g_spriteArray[0].X = 128;
		g_spriteArray[0].Y = 120;
		g_spriteArray[0].Type = BALLTYPE_PLAYER;
		g_spriteArray[0].Gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color); // allocate for each ball	
		dmaCopy(sprite_ballTiles, g_spriteArray[0].Gfx, 32 * 32 * 2);		// Copy the ball tiles to each ball in the array
		
		g_spriteArray[0].CircleDef = new b2CircleDef();
		g_spriteArray[0].BodyDef =  new b2BodyDef();
		
		g_spriteArray[0].CircleDef->radius = 24 / 2 * SCALE; 
		g_spriteArray[0].CircleDef->density = 0.5F; 
		g_spriteArray[0].CircleDef->friction = 0.2F; 
		g_spriteArray[0].CircleDef->restitution = 0.8F; 

		g_spriteArray[0].BodyDef->position.Set(g_spriteArray[0].X * SCALE, g_spriteArray[0].Y * SCALE);
		g_spriteArray[0].BodyDef->AddShape(g_spriteArray[0].CircleDef);
		
		g_spriteArray[0].Body = g_world->CreateBody(g_spriteArray[0].BodyDef);

	g_levelX = 0;
	g_levelY = 0;
	drawMap();

	float timeStep = 1.0f / 30.0f;
	int iterations = 1;
	char buffer[20];
	
	while(1)
	{
		b2Vec2 position = g_spriteArray[0].Body->GetOriginPosition();
		float rotationP = g_spriteArray[0].Body->GetRotation();
	
		sprintf(buffer, "%d X ",(int)(position.x * 10)) ;
		DrawString(buffer, 10, 21, false);
		sprintf(buffer, "%d Y ",192 - (int)(position.y * 10)) ;
		DrawString(buffer, 16, 21, false);			
		sprintf(buffer, "%d X SCRL",(int) g_levelX) ;
		DrawString(buffer, 0, 4, false);
		sprintf(buffer, "%d Y SCRL",(int) g_levelY) ;
		DrawString(buffer, 0, 5, false);	
		sprintf(buffer, "%d Rot",(int)(rotationP *100)) ;
		DrawString(buffer, 16, 20, false);		
		

		drawMap();
		
		scanKeys();						// Read button data
		int held = keysHeld();			// Used to calculate if a button is down
		
		// checks for left and right movement		(Use L/R and LEFT/RIGHT)
		
		b2Vec2 vel = g_spriteArray[0].Body->GetLinearVelocity();
		
		if (held & KEY_LEFT)
		{
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(-VELX, vel.y));
			g_spriteArray[0].Body->ApplyImpulse(b2Vec2(IMPX, 0), b2Vec2(0, 0));
		}
		else if (held & KEY_RIGHT)
		{
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(VELX, vel.y));
			g_spriteArray[0].Body->ApplyImpulse(b2Vec2(-IMPX, 0), b2Vec2(0, 0));
		}
		else if (held & KEY_UP)
		{
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, VELY));
			g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, IMPY), b2Vec2(0, 0));
		}
		else if (held & KEY_DOWN)
		{
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(vel.x, -VELY));
			g_spriteArray[0].Body->ApplyImpulse(b2Vec2(0, -IMPY), b2Vec2(0, 0));
		}
		else
		{
			g_spriteArray[0].Body->SetLinearVelocity(b2Vec2(0.0f, vel.y));
		}
	
		g_world->Step(timeStep, iterations);
		
		for(int i=0; i<BALLCOUNT; i++)
		{
			b2Vec2 position = g_spriteArray[i].Body->GetOriginPosition();
			float rotation = g_spriteArray[i].Body->GetRotation();
			
			oamRotateScale(&oamSub, i, degreesToAngle(rotation * 100), intToFixed(1, 8), intToFixed(1, 8));	// rotate the sprite
			
			g_spriteArray[i].X = (position.x / SCALE);			
			g_spriteArray[i].Y = 192 - BALLSIZE - ((float)position.y / SCALE);
			
			oamSet(&oamSub, i, g_spriteArray[i].X - BALLOFFSET, g_spriteArray[i].Y - BALLOFFSET, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, g_spriteArray[i].Gfx, i, false, false, false, false, false);
		};
		
		/* if (keysHeld() & KEY_L)
		{	
			for(int i=1; i<BALLCOUNT; i++)
			{
			b2Vec2 position = g_spriteArray[i].Body->GetOriginPosition();
			position.x = (rand() % 100) + 140;
			position.y =  192 - ((rand() % 100));		// should this not work?
			}
	
		} */

	
		// Wait for vblank
		swiWaitForVBlank();
		
		// Update temp oam to real oam (must be done during a vblank so it's put here just under the vblank wait)
		// oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	// Will never reach here
	return 0;
}
