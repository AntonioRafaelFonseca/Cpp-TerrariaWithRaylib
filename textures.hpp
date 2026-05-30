#pragma once
#include "raylib.h"
struct Textures
{
static inline Texture2D TextureAir;
static inline Texture2D TextureBark;
static inline Texture2D TextureDirt;
static inline Texture2D TextureGrass;
static inline Texture2D TextureHeart0;
static inline Texture2D TextureHeart1;
static inline Texture2D TextureLeafs;
static inline Texture2D TexturePlayer;
static inline Texture2D TextureSlot;
static inline Texture2D TextureSlotarrow;
static inline Texture2D TextureStone;
static inline Texture2D TextureTorch;

static void LoadAll() {
TextureAir = LoadTexture("imgs/AirTexture.png");
TextureBark = LoadTexture("imgs/BarkTexture.png");
TextureDirt = LoadTexture("imgs/DirtTexture.png");
TextureGrass = LoadTexture("imgs/GrassTexture.png");
TextureHeart0 = LoadTexture("imgs/heart0.png");
TextureHeart1 = LoadTexture("imgs/heart1.png");
TextureLeafs = LoadTexture("imgs/LeafsTexture.png");
TexturePlayer = LoadTexture("imgs/PlayerTexture.png");
TextureSlot = LoadTexture("imgs/Slot.png");
TextureSlotarrow = LoadTexture("imgs/SlotArrowTexture.png");
TextureStone = LoadTexture("imgs/StoneTexture.png");
TextureTorch = LoadTexture("imgs/TorchTexture.png");

}
static void UnloadAll() {
UnloadTexture(TextureAir);
UnloadTexture(TextureBark);
UnloadTexture(TextureDirt);
UnloadTexture(TextureGrass);
UnloadTexture(TextureHeart0);
UnloadTexture(TextureHeart1);
UnloadTexture(TextureLeafs);
UnloadTexture(TexturePlayer);
UnloadTexture(TextureSlot);
UnloadTexture(TextureSlotarrow);
UnloadTexture(TextureStone);
UnloadTexture(TextureTorch);
}
};
