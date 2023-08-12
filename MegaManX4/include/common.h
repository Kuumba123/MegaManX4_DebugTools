#ifndef COMMON_H
#define COMMON_H
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <stdbool.h>
#include <object.h>

#define WriteShort(addr, value) (*(short*)(addr) = (value))
#define WriteInt(addr, value) (*(int*)(addr) = (value))
#define WriteByte(addr, value) (*(char*)(addr) = (value))

enum PadButtons{
    PAD_L1 = 4,
    PAD_R1 = 8,
    PAD_L2 = 1,
    PAD_R2 = 2,
    PAD_SELECT = 0x100,
    PAD_START = 0x800,
    PAD_UP = 0x1000,
    PAD_RIGHT = 0x2000,
    PAD_DOWN = 0x4000,
    PAD_LEFT = 0x8000,
    PAD_TRIANGLE = 0x10,
    PAD_CIRCLE = 0x20,
    PAD_CROSS = 0x40,
    PAD_SQUARE = 0x80
};

extern ushort buttonsHeld;
extern ushort buttonsPressed;
extern u_char cursor;
extern void * freeArcP;

void ArcSeek(ushort id,u_char bufferId,void * bufferP);
void BinSeek(ushort id,void * dest);
void ClearAll(void);
void DrawLoad(bool showName ,u_char setTozero);
void DrawMain();
void DrawMMX4(void);
void EndSong(void);

void FadeIn(u_char speed);
void FadeOut(u_char speed);
void FileCollect();
extern char fadeDirection; //0 = done

void PlaySound(int slot,int id,objStruct *objP);
void PlaySong(byte id,byte vol);

char * strcpy(void * dest,void * src);
void * memcpy(void *dest, void *src,int length);
void * memset(void *dest, char *fillbyte,int length);

int printf(const char *fmt,...);

void SpawnText(ushort id,byte idk,byte flag);
void ThreadSleep(ushort frames);

#endif