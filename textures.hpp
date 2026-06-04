#pragma once
#include "raylib.h"
#include "block_type.hpp"
struct Textures
{
static inline Texture2D TextureAir;
static inline Texture2D TextureBackground;
static inline Texture2D TextureBark;
static inline Texture2D TextureCoal;
static inline Texture2D TextureCock;
static inline Texture2D TextureCrafter;
static inline Texture2D TextureDirt;
static inline Texture2D TextureGrass;
static inline Texture2D TextureHeart0;
static inline Texture2D TextureHeart1;
static inline Texture2D TextureLeaf;
static inline Texture2D TextureNo;
static inline Texture2D TexturePlayer;
static inline Texture2D TextureSlot;
static inline Texture2D TextureSlotarrow;
static inline Texture2D TextureStone;
static inline Texture2D TextureTorch;
static inline Texture2D TextureWoodenplank;

static void LoadAll() {
TextureAir = LoadTexture("imgs/AirTexture.png");
TextureBackground = LoadTexture("imgs/Background.png");
TextureBark = LoadTexture("imgs/BarkTexture.png");
TextureCoal = LoadTexture("imgs/CoalTexture.png");
TextureCock = LoadTexture("imgs/CockTexture.png");
TextureCrafter = LoadTexture("imgs/CrafterTexture.png");
TextureDirt = LoadTexture("imgs/DirtTexture.png");
TextureGrass = LoadTexture("imgs/GrassTexture.png");
TextureHeart0 = LoadTexture("imgs/heart0.png");
TextureHeart1 = LoadTexture("imgs/heart1.png");
TextureLeaf = LoadTexture("imgs/LeafTexture.png");
TextureNo = LoadTexture("imgs/NO.png");
TexturePlayer = LoadTexture("imgs/PlayerTexture.png");
TextureSlot = LoadTexture("imgs/Slot.png");
TextureSlotarrow = LoadTexture("imgs/SlotArrowTexture.png");
TextureStone = LoadTexture("imgs/StoneTexture.png");
TextureTorch = LoadTexture("imgs/TorchTexture.png");
TextureWoodenplank = LoadTexture("imgs/WoodenPlankTexture.png");

}
static void UnloadAll() {
UnloadTexture(TextureAir);
UnloadTexture(TextureBackground);
UnloadTexture(TextureBark);
UnloadTexture(TextureCoal);
UnloadTexture(TextureCock);
UnloadTexture(TextureCrafter);
UnloadTexture(TextureDirt);
UnloadTexture(TextureGrass);
UnloadTexture(TextureHeart0);
UnloadTexture(TextureHeart1);
UnloadTexture(TextureLeaf);
UnloadTexture(TextureNo);
UnloadTexture(TexturePlayer);
UnloadTexture(TextureSlot);
UnloadTexture(TextureSlotarrow);
UnloadTexture(TextureStone);
UnloadTexture(TextureTorch);
UnloadTexture(TextureWoodenplank);
}
};
inline Texture2D getTextureByType(BlockType type){
    if (type == BlockType::AIR)
    {
        return Textures::TextureAir;
    }
    else if (type == BlockType::BARK)
    {
        return Textures::TextureBark;
    }
    else if (type == BlockType::COAL)
    {
        return Textures::TextureCoal;
    }
    else if (type == BlockType::CRAFTER)
    {
        return Textures::TextureCrafter;
    }
    else if (type == BlockType::DIRT)
    {
        return Textures::TextureDirt;
    }
    else if (type == BlockType::GRASS)
    {
        return Textures::TextureGrass;
    }
    else if (type == BlockType::LEAF)
    {
        return Textures::TextureLeaf;
    }
    else if (type == BlockType::STONE)
    {
        return Textures::TextureStone;
    }
    else if (type == BlockType::TORCH)
    {
        return Textures::TextureTorch;
    }
    else if (type == BlockType::WOODENPLANK)
    {
        return Textures::TextureWoodenplank;
    }
return Textures::TextureNo;}