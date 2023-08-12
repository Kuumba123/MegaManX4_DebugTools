/*
 * 2nd file due to a lack of space...
 */
#include <common.h>
#include <gpu.h>
#define bufferSize 0x1058 //of single buffer
#define textBufferAddress 0x80175f50 //after APU stuff and just before ARC Buffer
#define maxTextCount ((bufferSize / 16) - 1)

int textCount = 0;
char hexDigits[] = "0123456789ABCDEF";
char lowerhexDigits[] = "0123456789abcdef";
DR_TPAGE fontTPages[2];

void ResetText()
{
    DrawSync(0);
    textCount = 0;
}

void DrawChar(char letter, int destX, int destY, byte clut)
{
    if (textCount < maxTextCount && letter != ' ')
    {
        letter -= 0x20;
        SPRT_8 *sp = (SPRT_8 *)(textCount * 16 + textBufferAddress + (bufferSize * buffer));
        setRGB0(sp, 128, 128, 128);
        setXY0(sp, destX, destY); // 32 Chars per row
        setUV0(sp, (letter & 0x1F) * 8, 0xE0 + (letter / 32) * 8);
        setClut(sp, 0x100 + clut * 16, 0x1F8);
        SetSprt8(sp);
        AddPrim(&fontTPages[buffer], sp);
        textCount += 1;
    }
}
#undef maxTextCount
#undef textBufferAddress
#undef bufferSize