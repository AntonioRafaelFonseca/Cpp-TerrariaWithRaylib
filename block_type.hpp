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
  TORCH
};

enum LightSourceType : unsigned char
{
  NOLIGHT,
  NATURAL,
  TYPETORCH
};

struct Block
{
  BlockType type;
  LightSourceType LightSource = NOLIGHT;
  unsigned char Brightness = 50;
  bool isSolid();
};

bool Block::isSolid()
{
  if (this->type == AIR || this->type == TORCH) return false;
  return true;
}