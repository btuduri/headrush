#include "Globals.h"

// Here are the real global values. Place a "g_" before so you know you're referencing a global
// This is the real definition

Sprite g_spriteArray[];

//
// X,Y coord of game map
//
float g_levelX = 0;
float g_levelY = 0;

//
// jump mechanics
//

bool g_jumpTrap = 0;
bool g_reJump = 0;

Sprite g_colSprite1;
Sprite g_colSprite2;
