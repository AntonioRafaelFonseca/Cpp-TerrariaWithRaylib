#pragma once


enum BlockType : unsigned char
{
  NONE,
  AIR,
  GRASS,
  DIRT,
  STONE,
  LEAF,
  BARK,
  TORCH,
  CRAFTER,
  COAL,
  WOODENPLANK,
  NORMALFENCE,
  CONECTEDFENCE
};

enum LightSourceType : unsigned char
{
  NOLIGHT,
  NATURAL,
  TYPETORCH
};

struct Block // 3 bytes instead of 8/9 idk
{
  BlockType type;
  LightSourceType LightSource = NOLIGHT;
  unsigned char Brightness = 50;
  inline bool isSolid();
};

struct SmallBlock // 1 byte
{
  BlockType type = AIR;
};

inline bool Block::isSolid()
{
  if (this->type == AIR || this->type == TORCH) return false;
  return true;
}