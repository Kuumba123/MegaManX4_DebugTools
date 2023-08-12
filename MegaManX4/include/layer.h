#ifndef LAYER_H
#define LAYER_H

void AssignLayer(int layer);

void DrawLayer(int layer);
void DumpActiveScreens();
void DumpLayerScreens(int layer); //BG1-3

void LoadTiles(int id,short x,short y);
void LoadTiles2(int id,short x,short y); //Not Permanet

void ResetLayerPointers(int layer);

typedef struct{
    u_char layer;
    ushort pad0;
    u_char pad1;
    u_char settings; // tttt ttti , T = TileCount I = Increament-Y (if 0 Increament-X instead)
    u_char pad2;
    short destX;
    short destY;
    ushort pad3;
    u_char*tableP;
    bool proceed;
    ushort pad4;
} tileSetStruct;
#endif