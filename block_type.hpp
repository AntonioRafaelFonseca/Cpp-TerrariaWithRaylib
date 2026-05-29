#pragma once


enum BlockType
{
  NONE,
  AIR,
  GRASS,
  DIRT,
  STONE
};


struct Block
{
  BlockType type;
  unsigned char Brightness = 50;
};