#pragma once


enum BlockType
{
  NONE,
  AIR,
  GRASS,
  DIRT,
  STONE,
  LEAF,
  BARK,
};

enum LightSourceType
{
  NOLIGHT,
  NATURAL,
  TORCH
};

struct Block
{
  BlockType type;
  LightSourceType LightSource = NOLIGHT;
  unsigned char Brightness = 50;
};