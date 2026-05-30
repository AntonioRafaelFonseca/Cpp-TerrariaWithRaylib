#pragma once
#include <vector>
#include "raylib.h"
#include "block.hpp" // O Player precisa de saber o que é o Block!

class Player
{
public:
  float x = 250;
  float y = 300;
  float vy = 0;
  int life = 10;
  Inventory inventory;
  
  void update(std::vector<std::vector<Block>> b, unsigned char& pb);


private:
  void updateY(bool contact)
  {
    if (!contact)
    {
      this->vy += this->G;
    }
    else
    {
      vy = 0;
    }
    this->y += this->vy;
  }
  float G = 1.0f;
};

