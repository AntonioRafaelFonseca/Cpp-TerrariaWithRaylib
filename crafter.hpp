#pragma once
#include <algorithm>
#include <iostream>
#include "block_type.hpp"

struct returnerValue
{
  BlockType type;
  int amount;
  returnerValue(BlockType t, int a)
  {
    this->type = t;
    this->amount = a;
  }
};
class Crafter
{
private:
  BlockType torch[5] = {WOODENPLANK, COAL, NONE, NONE, NONE};
  BlockType woodP[5] = {BARK, BARK, BARK, BARK, NONE};
  BlockType fence[5] = {WOODENPLANK, WOODENPLANK, NONE, NONE, NONE};
  int index = 0;
public:
  BlockType blocks[5] = {NONE, NONE, NONE, NONE, NONE};
  returnerValue craft()
  {
    if (std::equal(std::begin(blocks), std::end(blocks), std::begin(torch)))
    {
      std::fill(std::begin(blocks), std::end(blocks), BlockType::NONE);
      return {TORCH, 8};
    }
    if (std::equal(std::begin(blocks), std::end(blocks), std::begin(woodP)))
    {
      std::fill(std::begin(blocks), std::end(blocks), BlockType::NONE);
      return {WOODENPLANK, 8};
    }
    if (std::equal(std::begin(blocks), std::end(blocks), std::begin(fence)))
    {
      std::fill(std::begin(blocks), std::end(blocks), BlockType::NONE);
      return {NORMALFENCE, 4};
    }
    return {NONE, 0};
  }
  
  void add(BlockType type)
  {
    updateIndex();
    blocks[index] =  type;
  }
  void updateIndex()
  {
    for (int i=0;i<5;i++)
    {
      if(blocks[i] == NONE)
      {
        index = i;
        return;
      }
    }
  }
};
