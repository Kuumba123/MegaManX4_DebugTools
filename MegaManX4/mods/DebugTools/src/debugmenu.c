#include <common.h>
#include <object.h>
#include <gpu.h>
#include "tools.h"

#define rectCount ((int *)0x1f80011c)[0]
#define tileInfoP (((u_char *)*(int *)0x1f80000c))
#define layoutP ((u_char *)*(int *)0x1f800004)
#define screenDataP ((ushort *)*(int *)0x1f800008)
#define layoutWidth ((u_char *)0x80172224)[0]
#define layoutHeight ((u_char *)0x80173a28)[0]
#define tempPrimP ((int *)0x1f800108)[0]

static struct camStruct
{
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    bool display;
    char type;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    short X;
    undefined field11_0xc;
    undefined field12_0xd;
    short Y;
    undefined field14_0x10;
    undefined field15_0x11;
    undefined field16_0x12;
    undefined field17_0x13;
    undefined field18_0x14;
    undefined field19_0x15;
    short pastX;
    undefined field21_0x18;
    undefined field22_0x19;
    short pastY;
    short pastBorderR;
    short pastBorderL;
    short pastBorderB;
    short pastBorderT;
    short borderR;
    short borderL;
    short borderB;
    short borderT;
    undefined field32_0x2c;
    undefined field33_0x2d;
    undefined field34_0x2e;
    undefined field35_0x2f;
    undefined field36_0x30;
    undefined field37_0x31;
    undefined field38_0x32;
    undefined field39_0x33;
    undefined field40_0x34;
    undefined field41_0x35;
    undefined field42_0x36;
    undefined field43_0x37;
    undefined field44_0x38;
    undefined field45_0x39;
    undefined field46_0x3a;
    undefined field47_0x3b;
    undefined field48_0x3c;
    undefined field49_0x3d;
    undefined field50_0x3e;
    undefined field51_0x3f;
    short baseX;
    short baseY;
    bool enabled;
    undefined field55_0x45;
    undefined field56_0x46;
    undefined field57_0x47;
    undefined field58_0x48;
    undefined field59_0x49;
    byte normalOT;
    byte priorityOT;
    bool update;
    undefined field63_0x4d;
    undefined field64_0x4e;
    undefined field65_0x4f;
    undefined field66_0x50;
    undefined field67_0x51;
    undefined field68_0x52;
    undefined field69_0x53;
};
#define bglayers ((struct camStruct *)0x801419b0)
void DrawDebugText(ushort x, ushort y, byte clut, char *textP, ...);

struct toolsStruct tools;

DR_TPAGE shadowTPage[2];
TILE shadowRect[2];

void ResetLayerBuffer() // dumb game
{
    SPRT_16 *rect = (0x8015d9d0 + buffer * 0x4000);
    for (size_t i = 0; i < 0x400; i++)
    {
        rect[i].code = 0x7D;
        *(char *)((int)&rect[i].tag + 3) = 3;
    }
}
void ResetCollisionCheck(u_char *gameP)
{
    void (*determineClear)() = 0x800200d4;
    determineClear(gameP);
    if (gameP[0] != 4 && gameP[0] != 5)
    {
        tools.enableCollision = false;
    }
}
void DrawShadow()
{
    SetDrawTPage(&shadowTPage[buffer], 0, 0, GetTPageValue(0, 2, 0, 0));
    SetTile(&shadowRect[buffer]);
    SetSemiTrans(&shadowRect[buffer], 1);
    setRGB0(&shadowRect[buffer], 80, 80, 80);
    setWH(&shadowRect[buffer], 320, 240);
    setXY0(&shadowRect[buffer], 0, 0);
    AddPrim(&drawP->ot[0], &shadowTPage[buffer]);
    AddPrim(&shadowTPage[buffer], &shadowRect[buffer]);
}
void DebugModeMenu()
{
    DrawDebugText(13, 9, 2, "DEBUG MODE MENU");

    if (cursor == 0)
    {
        DrawDebugText(12, 11, 1, "PLAYER DEBUG MODE");
    }
    else
    {
        DrawDebugText(12, 11, 0, "PLAYER DEBUG MODE");
    }

    if (cursor == 1)
    {
        DrawDebugText(11, 13, 1, "ITEM SET/RESET MODE");
    }
    else
    {
        DrawDebugText(11, 13, 0, "ITEM SET/RESET MODE");
    }
    if (cursor == 2)
    {
        DrawDebugText(15, 15, 1, "XA TEST");
    }
    else
    {
        DrawDebugText(15, 15, 0, "XA TEST");
    }
    if (cursor == 3)
    {
        DrawDebugText(14, 17, 1, "DEBUG OVERLAY");
    }
    else
    {
        DrawDebugText(14, 17, 0, "DEBUG OVERLAY");
    }
    if (cursor == 4)
    {
        DrawDebugText(11, 19, 1, "SCROLL DEBUG MODE");
    }
    else
    {
        DrawDebugText(11, 19, 0, "SCROLL DEBUG MODE");
    }

    if ((buttonsPressed & PAD_DOWN) != 0)
    {
        cursor += 1;
        if (cursor > 4)
        {
            cursor = 0;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        if (cursor == 0)
        {
            cursor = 4;
        }
        else
        {
            cursor -= 1;
        }
    }
    if ((buttonsPressed & PAD_CROSS) != 0)
    {
        tools.mode = cursor + 1;
        cursor = 0;
    }

    // End (draw red background)
    SetDrawTPage(&shadowTPage[buffer], 0, 0, GetTPageValue(0, 0, 0, 0));
    SetTile(&shadowRect[buffer]);
    SetSemiTrans(&shadowRect[buffer], 1);
    setRGB0(&shadowRect[buffer], 100, 30, 60);
    setWH(&shadowRect[buffer], 200, 130);
    setXY0(&shadowRect[buffer], 60, 67);
    AddPrim(&drawP->ot[0], &shadowTPage[buffer]);
    AddPrim(&shadowTPage[buffer], &shadowRect[buffer]);
}
void PlayerDebugModeMenu()
{
#define aquiredWeapons ((u_char *)0x80141981)[0]
#define clearedStages ((u_char *)0x80172219)[0]
#define hp ((u_char *)0x80141924)[0]
#define maxHP ((u_char *)0x80172206)[0]
#define weapon ((u_char *)0x8014195b)[0]
#define ammoAddr 0x80141970
#define lives ((u_char *)0x80172204)[0]
#define checkpoint ((u_char *)0x801721dd)[0]
#define clear ((u_char *)0x801721cf)[0]
    DrawDebugText(10, 5, 0, "PLAYER DEBUG MODE");

    for (size_t i = 0; i < 8; i++)
    {
        bool val = (aquiredWeapons & (1 << i));
        DrawDebugText(6, 7 + i, 0, "Weapon %d:%d", i + 1, val);
        val = (clearedStages & (1 << i));
        DrawDebugText(20, 7 + i, 0, "Boss %d:%d", i + 1, val);
    }

    DrawDebugText(6, 17, 0, "HP:   %2d/%2d", hp, maxHP);
    DrawDebugText(6, 18, 0, "AMMO: %2d", *((u_char *)(ammoAddr + weapon)));
    DrawDebugText(6, 19, 0, "LIVES:%d", lives);
    DrawDebugText(6, 20, 0, "POINT:%d", checkpoint);
    DrawDebugText(6, 21, 0, "CLEAR:%X", clear);

    /*Pad Logic*/
    if ((buttonsPressed & PAD_DOWN) != 0)
    {
        cursor++;
        if (cursor > 12)
        {
            cursor = 0;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        if (cursor == 0)
        {
            cursor = 12;
        }
        else
        {
            cursor--;
        }
    }
    else if ((buttonsPressed & (PAD_LEFT + PAD_RIGHT)) != 0)
    {
        tools.mode2 ^= 1;
    }


    /*Cursor Specfic Logic*/
    if (cursor < 8)
    {
        if (tools.mode2 == 0)
        {
            DrawDebugText(16, 7 + cursor, 1, "<");
        }
        else
        {
            DrawDebugText(28, 7 + cursor, 1, "<");
        }

        if ((buttonsPressed & PAD_CROSS) != 0)
        {
            if (tools.mode2 == 0)
            {
                /*Modify Weapon*/
                aquiredWeapons ^= 1 << cursor;
            }
            else
            {
                /*Modify Boss Cleared*/
                clearedStages ^= 1 << cursor;
            }
        }
    }
    else
    {
        DrawDebugText(5, 17 + (cursor - 8), 1, ">");

        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            if (cursor == 8)
            {
                if (hp < maxHP)
                {
                    hp += 1;
                }
            }else if (cursor == 9)
            {
                if (*((char*)(ammoAddr + weapon)) < 48)
                {
                    *((char*)(ammoAddr + weapon)) += 1;
                }
                
            }else if (cursor == 10)
            {
                if (lives < 9)
                {
                    lives += 1;
                }
                
            }else if (cursor == 11)
            {
                checkpoint += 1;
            }
        } 
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            if (cursor == 8)
            {
                if (hp > 1)
                {
                    hp -= 1;
                }
            }else if (cursor == 9)
            {
                if (*((char*)(ammoAddr + weapon)) != 0)
                {
                    *((char*)(ammoAddr + weapon)) -= 1;
                }
                
            }else if (cursor == 10)
            {
                if (lives != 0)
                {
                    lives -= 1;
                }
            }else if (cursor == 11)
            {
                checkpoint -= 1;
            }
            
        }else if((buttonsPressed & PAD_CROSS))
        {
            if(cursor == 8)
            {
                hp = maxHP;
            }else if (cursor == 9)
            {
                *((u_char *)(ammoAddr + weapon)) = 42;
            }else if (cursor == 12)
            {
                clear ^= 1;
            }
            
            
        }
        
        
    }
#undef hp
#undef maxHP
#undef clear
#undef checkpoint
#undef lives
#undef weapon
#undef ammoAddr
#undef aquiredWeapons
#undef clearedStages
    DrawShadow();
}
void ItemSetModeMenu()
{
#define collectables ((ushort*)0x8017221a)[0]
#define parts ((u_char*)0x8014196f)[0]
    for (size_t i = 0; i < 12; i++)
    {
        if (i < 8)
        {
            DrawDebugText(7,4 + i,0,"HEART-%d:%d",i + 1, (collectables & (1 << i)) != 0);
        }else
        {
            DrawDebugText(7,5 + i,0,"TANK-%d:%d",i - 8, (collectables & (1 << (i + 4) )) != 0);
        }
    }
    DrawDebugText(7,18,0,"HEAD:%d",(parts & 1) != 0);
    DrawDebugText(7,19,0,"BODY:%d",(parts & 2) != 0);
    DrawDebugText(7,20,0,"ARMS:%d",(parts & 4) != 0);
    DrawDebugText(7,21,0,"LEGS:%d",(parts & 8) != 0);

    u_char newHpMax = *((u_char*)0x80172206);
    u_char newParts = parts;

    if ((buttonsPressed & PAD_UP) != 0)
    {
        if (cursor == 0)
        {
            cursor = 15;
        }else
        {
            cursor--;
        }
    }else if ((buttonsPressed & PAD_DOWN) != 0)
    {
        if (cursor == 15)
        {
            cursor = 0;
        }else
        {
            cursor++;
        }
        
    }
    
    
    if (cursor < 12)
    {
        if (cursor < 8)
        {
            DrawDebugText(16,4 + cursor,1, "<");
        }else
        {
            DrawDebugText(15,5 + cursor,1, "<");
        }
        

        if ((buttonsPressed & PAD_CROSS) != 0)
        {
            if (cursor < 8)
            {

                collectables ^= 1 << cursor;
                if ((collectables & ( 1 << cursor)) == 0)
                {
                    newHpMax -= 2;
                }else 
                {
                    newHpMax += 2;;
                }
                
            }else
            {
                collectables ^= 1 << (cursor + 4);
            }
            
        }
    }else if (cursor < 16)
    {
        DrawDebugText(13,18 + cursor - 12,1, "<");
        if ((buttonsPressed & PAD_CROSS) != 0)
        {
            newParts ^= 1 << (cursor - 12);
        }
        
    }
    *((u_char*)0x80172206) = newHpMax;
    *((u_char*)0x80172206 -1) = newHpMax;
    *((u_char*)0x80172207) = newParts;
    parts = newParts;
    
#undef collectables
#undef parts
    DrawShadow();
}
void XA_TestModeMenu()
{
#define MAX_SONGS 80
#define song tools.mode2
#define isStereo ((bool *)0x80171ea9)[0]
#define volume ((u_char *)0x80139524)[0]
#define songStartLBA ((uint *)0x80139540)[0]
#define songEndLBA ((uint *)0x80139544)[0]
#define songCurrentLBA ((uint *)0x8013953c)[0]
#define paused ((bool *)0x801441b8)[0]
    if (paused)
    {
        DrawDebugText(15, 17, 2, "(PAUSED)");
    }

    if (cursor == 0)
    {
        DrawDebugText(5, 3, 1, "SONG: %X", song);
    }
    else
    {
        DrawDebugText(5, 3, 0, "SONG: %X", song);
    }

    u_char stereoClut;
    if (cursor == 1)
    {
        stereoClut = 1;
    }
    else
    {
        stereoClut = 0;
    }
    if (isStereo)
    {
        DrawDebugText(5, 5, stereoClut, "STEREO");
    }
    else
    {
        DrawDebugText(5, 5, stereoClut, "MONO");
    }
    if (cursor == 2)
    {
        DrawDebugText(5, 7, 1, "VOLUME: %X", volume);
    }
    else
    {
        DrawDebugText(5, 7, 0, "VOLUME: %X", volume);
    }

    DrawDebugText(3, 20, 0, "S-LBA:%5X E-LBA:%5X C-LBA:%5X", songStartLBA, songEndLBA, songCurrentLBA);

    bool crossPressed = false;

    if ((buttonsPressed & PAD_CROSS) != 0)
    {
        crossPressed = true;
    }
    if ((buttonsPressed & PAD_TRIANGLE) != 0)
    {
        paused ^= 1;
    }

    // Move Cursor Check
    if ((buttonsPressed & PAD_DOWN) != 0)
    {
        cursor += 1;
        if (cursor > 2)
        {
            cursor = 0;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        cursor -= 1;
        if (cursor > 2)
        {
            cursor = 2;
        }
    }
    //....

    if (cursor != 2) // Change Songs
    {
        if (crossPressed)
        {
            if (cursor == 0)
            {
                PlaySong(song, volume);
            }
            else
            {
                isStereo ^= 1;
            }
        }
        else if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            song += 1;
            if (song > MAX_SONGS)
            {
                song = 0;
            }
        }
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            song -= 1;
            if (song > MAX_SONGS)
            {
                song = MAX_SONGS;
            }
        }
    }
    else
    {
        void (*setVolume)() = 0x80016420; // bad practice...

        if ((buttonsHeld & PAD_LEFT) != 0)
        {
            setVolume(volume - 1);
        }
        else if ((buttonsHeld & PAD_RIGHT) != 0)
        {
            setVolume(volume + 1);
        }
    }

    // End
    DrawShadow();
#undef MAX_SONGS
#undef song
#undef isStereo
#undef volume
#undef songStartLBA
#undef songEndLBA
#undef songCurrentLBA
#undef paused
}

void DebugOverlayMenu()
{
#define startY 9
#define startX 8
    DrawDebugText(10, 5, 0, "DEBUG OVERLAY");
    DrawDebugText(29, startY + cursor, 1, "<");
    for (size_t i = 0; i < 6; i++)
    {
        DrawDebugText(28, startY + i, 0, "%u", ((char *)&tools.showPlayerPos)[i]);
    }

    DrawDebugText(startX, startY, 0, "SHOW PLAYER CORD");
    DrawDebugText(startX, startY + 1, 0, "SHOW SCROLL");
    DrawDebugText(startX, startY + 2, 0, "SHOW COLLISION");
    DrawDebugText(startX, startY + 3, 0, "SHOW HITBOX");
    DrawDebugText(startX, startY + 4, 0, "HITBOX TYPE");
    DrawDebugText(startX, startY + 5, 0, "SHOW CAMERA TRIGGER");
#undef startY
#undef startX
    if ((buttonsPressed & PAD_CROSS) != 0)
    {
        if (cursor == 0)
        {
            tools.showPlayerPos ^= 1;
        }
        else if (cursor == 1)
        {
            tools.showScroll ^= 1;
        }
        else if (cursor == 2)
        {
            tools.enableCollision ^= 1;
            struct camStruct *bgInfo = 0x801419b0;
            bgInfo[0].update = true;
            bgInfo[1].update = true;
            bgInfo[2].update = true;
        }
        else if (cursor == 3)
        {
            tools.enableHitbox ^= 1;
        }
        else if (cursor == 4)
        {
            tools.hitboxType++;
            if (tools.hitboxType > 2)
            {
                tools.hitboxType = 0;
            }
        }
        else if (cursor == 5)
        {
            tools.showTrigger ^= 1;
        }
        /****End of Options*****/
    }
    else if ((buttonsPressed & PAD_DOWN) != 0)
    {
        if (cursor != 5)
        {
            cursor++;
        }
        else
        {
            cursor = 0;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        if (cursor != 0)
        {
            cursor--;
        }
        else
        {
            cursor = 5;
        }
    }
    // End
    DrawShadow();
}

void ScrollDebugModeMenu()
{
#if true
    DrawDebugText(10, 5, 0, "SCROLL DEBUG MODE");

#define startY 9
#define startX 8
    for (size_t i = 0; i < 3; i++)
    {
        DrawDebugText(startX, startY + i * 2, 0, "LAYER-%d X: %4X", i + 1, bglayers[i].X);
        DrawDebugText(startX, startY + i * 2 + 1, 0, "LAYER-%d Y: %4X", i + 1, bglayers[i].Y);
    }
    DrawDebugText(startX, startY + 6 + 1, 0, "BORDER-RIGHT:  %4X", bglayers[0].borderR);
    DrawDebugText(startX, startY + 7 + 1, 0, "BORDER-LEFT:   %4X", bglayers[0].borderL);
    DrawDebugText(startX, startY + 8 + 1, 0, "BORDER-BOTTOM: %4X", bglayers[0].borderB);
    DrawDebugText(startX, startY + 9 + 1, 0, "BORDER-TOP:    %4X", bglayers[0].borderT);

    for (size_t i = 0; i < 3; i++)
    {
        DrawDebugText(startX, startY + 12 + i, 0, "LAYER-%d DISPLAY:%d", i+1, bglayers[i].display);
    }
    
    if (cursor < 6)
    {
        DrawDebugText(23, startY + cursor, 1, "<");
    }else if(cursor < 10)
    {
        DrawDebugText(27, startY + 1 + cursor, 1, "<");
    }else
    {
        DrawDebugText(25, startY + 2 + cursor, 1, "<");
    }
    

    if ((buttonsPressed & PAD_DOWN) != 0)
    {
        cursor += 1;
        if (cursor > 12)
        {
            cursor = 0;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        if (cursor == 0)
        {
            cursor = 12;
        }
        else
        {
            cursor -= 1;
        }
    }
    int maxX = (layoutWidth - 1) << 8;
    int maxY = (layoutHeight - 1) << 8;

    if ((buttonsPressed & PAD_TRIANGLE) != 0)
    {
        bglayers[0].borderL = 0;
        bglayers[0].borderT = 0;
        bglayers[0].borderR = maxX;
        bglayers[0].borderB = maxY;
    }
    
    if (cursor < 6) // Modify Scroll
    {
        int i = cursor >> 1;
        if ((buttonsHeld & PAD_RIGHT) != 0)
        {
            if ((cursor & 1) == 0) // Modify X
            {
                if (bglayers[i].X < maxX)
                {
                    bglayers[i].X += 1;
                }
            }
            else
            {
                if (bglayers[i].Y < maxY)
                {
                    bglayers[i].Y += 1;
                }
            }
        }
        else if ((buttonsHeld & PAD_LEFT) != 0)
        {
            if ((cursor & 1) == 0) // Modify X
            {
                if (bglayers[i].X > 0)
                {
                    bglayers[i].X -= 1;
                }
            }
            else
            {
                if (bglayers[i].Y > 0)
                {
                    bglayers[i].Y -= 1;
                }
            }
        }
    }
    else if(cursor < 10)
    {
        if ((buttonsHeld & PAD_RIGHT) != 0)
        {
            /*Probably shouldnt be using a million if statements...*/
            if (cursor == 6)
            {
                if (bglayers[0].borderR < maxX)
                {
                    bglayers[0].borderR += 1;
                }
            }
            else if (cursor == 7)
            {
                if (bglayers[0].borderL < maxX)
                {
                    bglayers[0].borderL += 1;
                }
            }
            else if (cursor == 8)
            {
                if (bglayers[0].borderB < maxY)
                {
                    bglayers[0].borderB += 1;
                }
            }
            else if (cursor == 9)
            {
                if (bglayers[0].borderT < maxY)
                {
                    bglayers[0].borderT += 1;
                }
            }
        }
        else if ((buttonsHeld & PAD_LEFT) != 0)
        {
            if (cursor == 6)
            {
                if (bglayers[0].borderR != 0)
                {
                    bglayers[0].borderR -= 1;
                }
            }
            else if (cursor == 7)
            {
                if (bglayers[0].borderL != 0)
                {
                    bglayers[0].borderL -= 1;
                }
            }
            else if (cursor == 8)
            {
                if (bglayers[0].borderB != 0)
                {
                    bglayers[0].borderB -= 1;
                }
            }
            else if (cursor == 9)
            {
                if (bglayers[0].borderT != 0)
                {
                    bglayers[0].borderT -= 1;
                }
            }
        }
    }else
    {
        if ((buttonsPressed & PAD_CROSS) != 0)
        {
            bglayers[cursor - 10].display ^= 1;
        }
        
    }

#undef startX
#undef startY
    DrawShadow();
#endif
}
void (*debugMenuTable[])() = {DebugModeMenu, PlayerDebugModeMenu, ItemSetModeMenu, XA_TestModeMenu, DebugOverlayMenu, ScrollDebugModeMenu};
void MenuCheck()
{
    if (tools.opended)
    {
        debugMenuTable[tools.mode]();
    }
    else
    {
        void (*runVarious)() = 0x80021158; // bad practice...
        runVarious();

// Overlay Debugging Info
#define startX 6
        int y = 3;

        if (tools.showPlayerPos)
        {
            DrawDebugText(startX, y, 1, "PL-X: %4X", *((ushort *)0x801418d2));
            DrawDebugText(startX, y + 1, 1, "PL-Y: %4X", *((ushort *)0x801418d6));
            y += 2;
        }
        if (tools.showScroll)
        {
            struct camStruct *bgInfo = 0x801419b0;
            for (size_t i = 0; i < 3; i++)
            {
                DrawDebugText(startX, y, 1, "BG%d-X:%4X", i + 1, (ushort)bgInfo[i].X);
                DrawDebugText(startX, y + 1, 1, "BG%d-Y:%4X", i + 1, (ushort)bgInfo[i].Y);
                y += 2;
            }
        }

#undef startX
    }
    if ((buttonsPressed & PAD_SELECT) != 0)
    {
        tools.opended ^= 1;
        tools.mode = 0;
        tools.mode2 = 0;
        tools.mode3 = 0;
        cursor = 0;
    }
}

#define posX *(ushort *)((int)&objP->x + 2)
#define posY *(ushort *)((int)&objP->y + 2)
DR_TPAGE hitboxTPage[2];

void DrawCollisionHitbox(objStruct *objP, POLY_F4 *polyP)
{
    short left;
    short right;
    if (objP->flip == 0)
    {
        left = posX + objP->collideP->offsetX - objP->collideP->width;
        right = posX + objP->collideP->offsetX + objP->collideP->width;
    }
    else
    {
        left = posX - objP->collideP->offsetX - objP->collideP->width;
        right = posX - objP->collideP->offsetX + objP->collideP->width;
    }
    left -= bglayers[objP->collideLayer].X;
    right -= bglayers[objP->collideLayer].X;
    short top = posY + objP->collideP->offsetY - objP->collideP->height - bglayers[objP->collideLayer].Y;
    short bottom = posY + objP->collideP->height + objP->collideP->offsetY - bglayers[objP->collideLayer].Y;

    // Monochrome Transperent Quad
    setXY4(polyP, left, top, right, top, left, bottom, right, bottom);
    *(char *)((int)&polyP->tag + 3) = 5;
    polyP->code = 0x2A;
    setRGB0(polyP, 0, 0xAF, 0);
    AddPrim(&hitboxTPage[buffer], polyP);
}
void DrawContact(objStruct *objP, POLY_F4 *polyP)
{
    short left;
    short right;
    if (objP->flip == 0)
    {
        left = posX + objP->contactP->offsetX;
        right = posX + objP->contactP->offsetX + objP->contactP->width + 1;
    }
    else
    {
        left = posX - objP->contactP->offsetX;
        right = posX - objP->contactP->offsetX - objP->contactP->width - 1;
    }
    left -= bglayers[objP->collideLayer].X;
    right -= bglayers[objP->collideLayer].X;
    short top = posY + objP->contactP->offsetY - bglayers[objP->collideLayer].Y;
    short bottom = posY + objP->contactP->height + objP->contactP->offsetY - bglayers[objP->collideLayer].Y;

    // Monochrome Transperent Quad
    setXY4(polyP, left, top, right, top, left, bottom, right, bottom);
    *(char *)((int)&polyP->tag + 3) = 5;
    polyP->code = 0x2A;
    setRGB0(polyP, 175, 0, 0);
    AddPrim(&hitboxTPage[buffer], polyP);
}
void DrawContact2(objStruct *objP, POLY_F4 *polyP)
{
    short left;
    short right;
    if (objP->flip == 0)
    {
        left = posX + objP->contactP2->offsetX;
        right = posX + objP->contactP2->offsetX + objP->contactP2->width + 1;
    }
    else
    {
        left = posX - objP->contactP2->offsetX;
        right = posX - objP->contactP2->offsetX - objP->contactP2->width - 1;
    }
    left -= bglayers[objP->collideLayer].X;
    right -= bglayers[objP->collideLayer].X;
    short top = posY + objP->contactP2->offsetY - bglayers[objP->collideLayer].Y;
    short bottom = posY + objP->contactP2->height + objP->contactP2->offsetY - bglayers[objP->collideLayer].Y;

    // Monochrome Transperent Quad
    setXY4(polyP, left, top, right, top, left, bottom, right, bottom);
    *(char *)((int)&polyP->tag + 3) = 5;
    polyP->code = 0x2A;
    setRGB0(polyP, 191, 191, 0);
    AddPrim(&hitboxTPage[buffer], polyP);
}
void DetermineDrawHitbox(objStruct *objP) // for MegaMan & Weapons
{
    if (rectCount > 999 || objP->flags == 0 || !objP->display)
    {
        return;
    }
    if (tools.hitboxType == 0) // Draw Collision
    {
        if (objP->collideP != 0)
        {
            DrawCollisionHitbox(objP, tempPrimP);
            tempPrimP += sizeof(POLY_F4);
            rectCount += 2;
        }
    }
    else if (tools.hitboxType == 1)
    {
        if (objP->contactP != 0)
        {
            DrawContact(objP, tempPrimP);
            tempPrimP += sizeof(POLY_F4);
            rectCount += 2;
        }
    }
    else
    {
        if (objP->contactP2 != 0)
        {
            DrawContact2(objP, tempPrimP);
            tempPrimP += sizeof(POLY_F4);
            rectCount += 2;
        }
    }
}
void DetermineDrawHitbox2(objStruct *objP) // for Everything else
{
    if (rectCount > 999 || objP->flags == 0 || !objP->display)
    {
        return;
    }
    if (tools.hitboxType == 0) // Draw Collision
    {
        if (objP->collideP != 0)
        {
            DrawCollisionHitbox(objP, tempPrimP);
            tempPrimP += sizeof(POLY_F4);
            rectCount += 2;
        }
    }
    else if (tools.hitboxType == 1) // Draw Weapon Hitboxes
    {
        if (objP->contactP2 != 0)
        {
            DrawContact2(objP, tempPrimP);
            tempPrimP += sizeof(POLY_F4);
            rectCount += 2;
        }
    }
    else
    {
        if (objP->contactP != 0) // Draw Player Hitboxes
        {
            DrawContact(objP, tempPrimP);
            tempPrimP += sizeof(POLY_F4);
            rectCount += 2;
        }
    }
}
#undef posX
#undef posY
#undef bgLayers
void DrawHitboxCheck()
{
    if (!tools.enableHitbox)
    {
        return;
    }
#define megaAddr 0x801418c8

#define weaponAddr 0x801406f8
#define weaponSize 0x9C
#define weaponSlotCount 0x10

#define mainAddr 0x8013bed0
#define mainSize 0x9C
#define mainSlotCount 0x30

#define shotAddr 0x8013f328
#define shotSize 0x9C
#define shotSlotsCount 0x20

#define itemAddr 0x80165a30
#define itemSize 0x8C
#define itemSlotsCount 0x20
    /*Check Object Memory*/
#define objP ((int *)0x1f80004c)[0]
    DetermineDrawHitbox(megaAddr);

    /*Check Weapon Objects*/
    objP = weaponAddr;
    for (size_t i = 0; i < weaponSlotCount; i++)
    {
        DetermineDrawHitbox(objP);
        objP += weaponSize;
    }

    /*Check Shot Object*/
    objP = shotAddr;
    for (size_t i = 0; i < shotSlotsCount; i++)
    {
        DetermineDrawHitbox2(objP);
        objP += shotSize;
    }

    /*Check Main Objects*/
    objP = mainAddr;
    for (size_t i = 0; i < mainSlotCount; i++)
    {
        DetermineDrawHitbox2(objP);
        objP += mainSize;
    }
    DetermineDrawHitbox(0x80173a30); // Ride Armor

#undef objP

#undef megaAddr

#undef weaponAddr
#undef weaponSize
#undef weaponSlotCount

#undef mainAddr
#undef mainSize
#undef mainSlotCount

#undef shotAddr
#undef shotSize
#undef shotSlotsCount

#undef itemAddr
#undef itemASize
#undef itemSlotsCount
}
static struct OUTLINE
{
    u_long *tag;
    u_char r0, g0, b0;
    u_char code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
    short x4, y4;
    u_long pad;
};
void DrawTriggerCheck()
{
    int stageid = ((char *)0x801721cc)[0];
    int mid = ((char *)0x801721cd)[0];
    if (!tools.showTrigger || (stageid * 2 + mid) > 25)
    {
        return;
    }
#define effectAddr 0x80142f98
#define effectSize 0x30
#define effectSlotCount 0x20
#define borderTriggerTableAddr 0x8010ae0c
    objStruct *objP = effectAddr;
    for (size_t i = 0; i < effectSlotCount; i++)
    {
        if (objP->flags != 0 && objP->id == 0 && rectCount < 1000)
        {
            int *tableP = *((int *)(borderTriggerTableAddr + (stageid * 2 + mid) * 4));

            if (tableP != 0)
            {
                short *triggerP = *((int *)(tableP + objP->stageVar));

                /*Get Trigger Sides*/
                short right = triggerP[0] - bglayers[0].X;
                short left = triggerP[1] - bglayers[0].X;
                short bottom = triggerP[2] - bglayers[0].Y;
                short top = triggerP[3] - bglayers[0].Y;

                /*Draw Line Outline*/
                struct OUTLINE *outline = tempPrimP;
                outline->code = 0x48;
                *(char *)((int)&outline->tag + 3) = 7;

                // Define Sides
                setXY4(outline, left, top, right, top, right, bottom, left, bottom);
                outline->x4 = left;
                outline->y4 = top;
                outline->pad = 0x55555555;

                AddPrim(&hitboxTPage[buffer], outline);
                setRGB0(outline, 0, 0, 0xFF);

                rectCount += 4; // adjust later ...
                tempPrimP += sizeof(struct OUTLINE);

                /*Draw Monochrome Quad*/
                POLY_F4 *polyP = tempPrimP;
                setRGB0(polyP, 0, 0xE6, 0);
                setXY4(polyP, left, top, right, top, left, bottom, right, bottom);
                *(char *)((int)&polyP->tag + 3) = 5;
                polyP->code = 0x2A;
                AddPrim(&hitboxTPage[buffer], polyP);
                rectCount += 2;
                tempPrimP += sizeof(POLY_F4);
            }
        }
        objP = (char *)((int)objP + effectSize);
    }
#undef borderTriggerTableAddr
#undef effectSlotCount
#undef effectAddr
#undef effectSize
}

DR_TPAGE collisionTPage[2];
void DrawCollision()
{
    struct camStruct *bgInfo = 0x801419b0;
    DR_TPAGE *tpages = &collisionTPage[buffer];
    void *startP = tpages;
    SetDrawTPage(tpages, 0, 0, GetTPageValue(0, 0, 0xF, 1));
    AddPrim(&drawP->ot[1], tpages);
    int layerX = (int)bgInfo[0].X;
    int layerY = (int)bgInfo[0].Y;

    for (size_t y = 0; y < 16; y++)
    {
        for (size_t x = 0; x < 21; x++)
        {
            /* code */
            int x16 = layerX + x * 16;
            int y16 = layerY + y * 16;

            if ((uint)x16 > ((layoutWidth - 0) << 8))
            {
                x16 = (layoutWidth - 0) << 8;
            }
            if ((uint)y16 > ((layoutHeight - 0) << 8))
            {
                y16 = (layoutHeight - 0) << 8;
            }

            uint offset = (x16 >> 8) + (y16 >> 8) * layoutWidth;
            int screenId = layoutP[offset];
            ushort tileVal = screenDataP[screenId * 0x100 + ((x16 & 0xF0) >> 4) + (y16 & 0xF0)];

            if (tileVal != 0)
            {
                if (rectCount > 999)
                {
                    return;
                }
                ushort tileId = tileVal & 0x3FFF;

                /*Get Various Tile Id Info*/
                u_char collision = tileInfoP[tileId * 4] & 0x3F;

                SPRT_16 *rect = tempPrimP;

                /*RAW Textured Rectangle*/
                *(u_char *)((int)&rect->tag + 3) = 3;
                rect->code = 0x7D;

                if (collision != 0x3F)
                {
                    setUV0(rect, (collision & 0xF) * 16, (collision & 0xF0) + 192);
                }
                else
                {
                    setUV0(rect, 240, 176);
                }
                setClut(rect, 0x100, 0x1F8);
                /*Calculate Draw Location*/
                int innerX = x16 & 0xF;
                int innerY = y16 & 0xF;
                setXY0(rect, x * 16 - innerX, y * 16 - innerY);

                AddPrim(startP, rect);

                rectCount += 1;
                tempPrimP += sizeof(SPRT_16);
            }
        }
    }
}
void DrawDebugOverlay()
{
    if (tools.enableCollision)
    {
        SetDrawTPage(&hitboxTPage[buffer], 0, 0, GetTPageValue(0, 0, 0, 0));
        AddPrim(&drawP->ot[1], &hitboxTPage[buffer]);

        DrawCollision();
        DrawHitboxCheck();
        DrawTriggerCheck();
    }
}
#undef rectCount
#undef tileInfoP
#undef layoutP
#undef screenDataP
#undef layoutWidth
#undef layoutHeight
#undef tempPrimP