#ifndef OBJECT_H
#define OBJECT_H
#include <sys/types.h>
#include <stdbool.h>
typedef unsigned char   undefined;
typedef unsigned char    byte;

typedef struct objStruct objStruct, *PobjStruct;

typedef struct enemyStruct enemyStruct, *PenemyStruct;

typedef struct animeFrameStruct animeFrameStruct, *PanimeFrameStruct;

typedef struct
{
    char offsetX;
    char offsetY;
    u_char width;
    u_char height;
}hitboxS;


struct animeFrameStruct {
    byte timer;
    byte flag;
    char nextIndex; /* multiples of 4 */
    byte sprtFrame;
};

struct objStruct {
    byte flags; /* alive */
    byte id;
    byte stageVar;
    bool display;
    byte act;
    byte act2;
    byte act3;
    byte act4;
    int x;
    int y;
    struct enemyStruct * enemyP; /* for enemy spawner & other things */
    byte collideLayer; /* witch BG Layer for collision */
    byte flip;
    byte priority;
    byte anime;
    int pastX;
    int pastY;
    int speedX;
    int speedY;
    int accelX;
    int accelY;
    struct animeFrameStruct * animeTableP;
    byte * frameP;
    byte * texP;
    byte * sprtDataP;
    ushort texCord;
    ushort clutCord;
    struct animeFrameStruct animeInfo;
    byte newAnimeF;
    undefined field29_0x49;
    undefined field30_0x4a;
    undefined field31_0x4b;
    undefined field32_0x4c;
    undefined field33_0x4d;
    undefined field34_0x4e;
    undefined field35_0x4f;
    hitboxS * contactP; /* hitbox pointer for ContactO */
    hitboxS * contactP2; /* with other things that damage it */
    int * damageTableP; /* damage table Pointer (when hit) */
    byte hp;
    byte hp_old;
    undefined field41_0x5e;
    undefined field42_0x5f;
    byte damage;
    byte iframes;
    undefined field45_0x62;
    undefined field46_0x63;
    undefined field47_0x64;
    undefined field48_0x65;
    undefined field49_0x66;
    undefined field50_0x67;
    hitboxS * collideP; /* for collison & hitbox */
    ushort innerX;
    ushort innerY;
    byte collideF; /* flags for BTLR */
    undefined field55_0x71;
    undefined field56_0x72;
    undefined field57_0x73;
    undefined field58_0x74;
    undefined field59_0x75;
    undefined field60_0x76;
    undefined field61_0x77;
    undefined field62_0x78;
    undefined field63_0x79;
    byte hitboxDisableF; /* for Contact Weapon */
    undefined field65_0x7b;
    undefined field66_0x7c;
    undefined field67_0x7d;
    undefined field68_0x7e; //Start of Free Vars (does not include Weapons)
    undefined field69_0x7f;
    undefined field70_0x80;
    undefined field71_0x84;
    undefined field72_0x85;
    undefined field73_0x86;
    undefined field74_0x87;
    undefined field75_0x88;
    undefined field76_0x89;
    undefined field77_0x8A;
    undefined field78_0x8b;
    undefined field79_0x8c;
    undefined field80_0x8d;
    undefined field81_0x8e;
    undefined field82_0x8f;
    undefined field83_0x90;
    undefined field84_0x91;
    undefined field85_0x92;
    undefined field86_0x93;
    undefined field87_0x94;
    undefined field88_0x95;
    undefined field89_0x96;
    undefined field90_0x97;
    undefined field91_0x98;
    undefined field92_0x99;
    undefined field93_0x9a;
    undefined field94_0x9b;
};


struct enemyStruct {
    byte spawned;
    byte Id;
    byte var;
    byte type;
    ushort X;
    ushort Y;
};

/*Define Functions*/
void AnimeAdvance(objStruct *objP);

void CollideCheck(objStruct *objP);
int ContactMega(objStruct *objP);
bool ContactObject();
int ContactWeapon(objStruct *objP); //Returns Weapon Id+1, -1 if object is dead

void DeleteObject(objStruct *objP);
void DeleteObject2(objStruct *objP);
void DisplayObject(objStruct *objP);
void DisplayObject2(objStruct *objP, short width, short height);
void DisplayObject3(objStruct *objP);

objStruct* GetEffectObject();
objStruct* GetItemObject();
objStruct* GetMainObject();
objStruct* GetMiscObject();
int GetRNG();
objStruct* GetShotObject();
objStruct* GetVisualObject();

void LockMega(byte lockType, byte flip);

void MoveObject(objStruct *objP);

bool OffScreenCheck(objStruct* objP);
bool OffScreenCheck2(objStruct* objP, short rangeX, short rangeY);

u_char PlayBossSong();

void SetAnime(objStruct *objP, int anime);
void SetAnimeFrame(objStruct *objP, int anime, int frame);
void SpawnExplosion(objStruct *objP);
void SpawnJunk(u_char junkCount, void *idk, objStruct *objP);

void UnlockMega();
#endif
