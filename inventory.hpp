#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include "block_type.hpp"
#include "CustomData.hpp"
struct InventorySpace
{
  Block Item = {.type = BlockType::NONE};
  uint8_t amount = 0;
  bool full = false;
};

class Inventory
{
public:
  static const int size = 5;
  InventorySpace inventory[size];
  SlotIndex selected = 0;
  InventorySpace at(int n)
  {
    if(n>5 || n<0)
    {
      std::cerr << "Invalid value \nmin: 0\nmax:5";
      exit(1);
    }
    return inventory[n];
  }
};


void updateInventory(Inventory& inv)
{
  for(int i=0;i< sizeof(inv.inventory)/sizeof(inv.inventory[0]);i++)
  {
    InventorySpace& s = inv.inventory[i];

    if(s.amount <= 0)
    {
      s.amount = 0;
      s.Item.type = BlockType::NONE;
    }
    if (s.amount >= 32)
    {
      s.amount = 32;
      s.full = true;
    }
    else
    {
      s.full = false;
    }
  }
}