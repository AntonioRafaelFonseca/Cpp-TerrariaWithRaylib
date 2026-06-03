#pragma once
#include <vector>
#include "raylib.h"
#include "block.hpp" // O Player precisa de saber o que é o Block!

class Player
{
public:
  float x = 3*32;
  float y = 100;
  float vy = 0;
  int life = 10;
  float G = 1.0f;
  Inventory inventory;
  LayerIndex layer = { 0b00000001 };

  void gotoSurface()
  {
    this->y = 10*32;
  }
  void update(Blocks& b, unsigned char& pb)
  {
    if(IsKeyPressed(KEY_UP))
    {
      this->layer+=1;
      DrawText("Pressed AND Updated  \t Player-> Line 26", 10, 10, 10, BLACK);
    }

    float speed = 3.0f;
    float Jspeed = -12.0f;

    this->vy += this->G;

    int xindex = (int)this->x / 32;
    int yindex = (int)this->y / 32;

    pb = b.peek(xindex, yindex).Brightness;
    
    if (IsKeyDown(KEY_D))
    {
      int xDireita = (int)(this->x-5) / 32;
      if (!b.peek(xDireita + 1, yindex).isSolid()) this->x += speed;
    }
    if (IsKeyDown(KEY_A))
    {
      int xEsquerda = (int)(this->x+32) / 32;
      if (!b.peek(xEsquerda - 1, yindex).isSolid()) this->x -= speed;
    }
    
    yindex = (int)this->y / 32;
    bool contact = false;
    
    if (b.peek((int)(this->x + 4) / 32, yindex + 1).isSolid()) contact = true;
    if (!contact)
    {
      if (b.peek((int)(this->x + 20) / 32, yindex + 1).isSolid()) contact = true;
    }
    
    if (contact && this->vy > 0)
    {
      this->vy = 0;
      this->y = (yindex * 32); // Cola o boneco no topo do bloco de forma exata
    }
    
    int xindexTeto = (int)(this->x + 12) / 32;
    if (b.peek(xindexTeto, yindex - 1).isSolid() && this->vy < 0)
    {
      this->vy = 0;
      this->y = (yindex * 32); // Impede de atravessar o teto
    }

    if (IsKeyPressed(KEY_W) && contact)
    {
      if(!b.peek(xindexTeto, yindex - 1).isSolid()) this->vy = Jspeed;
    }
    
    this->y += this->vy;
  }
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
};

