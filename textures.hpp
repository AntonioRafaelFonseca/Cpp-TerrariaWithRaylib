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

static void LoadAll() {
TextureAir = LoadTexture("AirTexture.png");
TextureBark = LoadTexture("BarkTexture.png");
TextureDirt = LoadTexture("DirtTexture.png");
TextureGrass = LoadTexture("GrassTexture.png");
TextureHeart0 = LoadTexture("heart0.png");
TextureHeart1 = LoadTexture("heart1.png");
TextureLeafs = LoadTexture("LeafsTexture.png");
TexturePlayer = LoadTexture("PlayerTexture.png");
TextureSlot = LoadTexture("Slot.png");
TextureSlotarrow = LoadTexture("SlotArrowTexture.png");
TextureStone = LoadTexture("StoneTexture.png");

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
}
};
