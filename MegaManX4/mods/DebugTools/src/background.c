#include <common.h>
#include <gpu.h>
#include <layer.h>
#include "tools.h"

#define rectCount ((int*)0x1f80011c)[0]
#define tempPrimP ((int*)0x1f800108)[0]
#define rectBufferAddress 0x8015d9d0
static struct camStruct {
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
void ResetLayerBuffer();
void DrawDebugOverlay();
extern struct toolsStruct tools;
void NewDrawBackground()
{
    struct camStruct * bgInfo = 0x801419b0;
    rectCount = 0;
    tempPrimP = rectBufferAddress + buffer * 0x4000;
    if(!tools.enableCollision)
    {
        ResetLayerBuffer();
        
        for (size_t i = 0; i < 3; i++)
        {
            if(bgInfo[i].update){
                DumpLayerScreens(i);
            }
        }
        DumpActiveScreens();
        /*
        * TODO: maybe add back Intro Blue Rect effect...
        */
        for (size_t i = 0; i < 3; i++)
        {
            if(bgInfo[i].display)
            {
                ResetLayerPointers(i);
                DrawLayer(i);
                AssignLayer(i);
            }
        }
    }else
    {
        DrawDebugOverlay();
    }

}
#undef rectCount
#undef tempPrimP