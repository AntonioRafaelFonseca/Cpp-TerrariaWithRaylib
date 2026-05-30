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

enum LightSource
{
  NOLIGHT,
  NATURAL,
  TORCH
};

struct Block
{
  BlockType type;
  LightSource LightSource = NOLIGHT;
  unsigned char Brightness = 50;
};