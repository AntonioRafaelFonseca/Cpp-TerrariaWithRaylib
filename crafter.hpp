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
  BlockType torch[5] = {BARK, STONE, NONE, NONE, NONE};
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
