/*---------------------------------------------------------------------------------

Simple sprite demo
-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define PI 3.14159265

//---------------------------------------------------------------------------------
int main(void) {
	//---------------------------------------------------------------------------------
	int i = 0;
    touchPosition touch;
    
    int keys_pressed, keys_held, keys_released;
    
    typedef struct {
       unsigned char x;
       unsigned char y;
       unsigned char speed;
       double angle;
    } ball;
    
    ball b = {SCREEN_WIDTH / 2 - 1 - 4, SCREEN_HEIGHT / 2 - 1 - 4, 1, 45.0};
    
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	u16* gfxSub = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);

	for(i = 0; i < 16 * 16 / 2; i++)
	{
		gfx[i] = 1 | (1 << 8);
		gfxSub[i] = 1 | (1 << 8);
	}

	SPRITE_PALETTE[1] = RGB15(31,31,31);    // White
	SPRITE_PALETTE_SUB[1] = RGB15(0,31,0);

	while(1) {
        
		scanKeys();
        
        keys_pressed = keysDown();
        keys_held = keysHeld();
        keys_released = keysUp();
        
		if(keysHeld() & KEY_TOUCH) {
			touchRead(&touch);
		}

        if (keys_pressed & KEY_UP) {
            b.y = b.y - 1;
        } else if (keys_pressed & KEY_DOWN) {
            b.y = b.y + 1;
        }
        
        b.x = b.x + (unsigned char) round(b.speed * cos(b.angle * PI / 180.0));
        b.y = b.y + (unsigned char) round(b.speed * sin(b.angle * PI / 180.0));
        
		oamSet(&oamMain, //main graphics engine context
			0,           //oam index (0 to 127)  
			b.x, b.y,   //x and y pixle location of the sprite
			0,                    //priority, lower renders last (on top)
			0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_16x16,     
			SpriteColorFormat_256Color, 
			gfx,                  //pointer to the loaded graphics
			-1,                  //sprite rotation data  
			false,               //double the size when rotating?
			false,			//hide the sprite?
			false, false, //vflip, hflip
			false	//apply mosaic
			);              
		
		
		oamSet(&oamSub, 
			0, 
			touch.px, 
			touch.py, 
			0, 
			0,
			SpriteSize_16x16, 
			SpriteColorFormat_256Color, 
			gfxSub, 
			-1, 
			false, 
			false,			
			false, false, 
			false	
			);              
	
		swiWaitForVBlank();

		
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0;
}