#include <iostream>
#include <cmath>

#include "raylib.h"
#include "player.hpp"
#include "inventory.hpp"
#include "perlin.hpp"
#include "textures.hpp"

enum GameState
{
  RUNNING,
  PAUSED,
  MENU,
  NOT_RUNING
};

bool inBounds(int n, int min, int max)
{
  if(n<max && n>=min) return true;
  return false;
}

void drawPlayer(Player& p)
{
  for (int i=0;i<p.life;i++)
  {
    if (i%2==0) DrawTexture(Textures::TextureHeart0, i*32-10, 16, WHITE);
    else DrawTexture(Textures::TextureHeart1, i*32-42, 16, WHITE);
  }
  for(int i=0;i<5;i++)
  {
    DrawTexture(Textures::TextureSlotarrow, p.inventory.selected*32, (float)GetScreenHeight()-64, WHITE);
    DrawTexture(Textures::TextureSlot, i*Textures::TextureSlot.width, GetScreenHeight()-Textures::TextureSlot.height, WHITE);
    if(p.inventory.inventory[i].Item.type == DIRT)
    {
      DrawTextureEx(Textures::TextureDirt, {(float)i*Textures::TextureSlot.width, (float)GetScreenHeight()-Textures::TextureSlot.height}, 0.0f, 0.8, WHITE);
    }
    else if(p.inventory.inventory[i].Item.type == GRASS)
    {
      DrawTextureEx(Textures::TextureGrass, {(float)i*Textures::TextureSlot.width, (float)GetScreenHeight()-Textures::TextureSlot.height}, 0.0f, 0.8, WHITE);
    }
    else if(p.inventory.inventory[i].Item.type == STONE)
    {
      DrawTextureEx(Textures::TextureStone, {(float)i*Textures::TextureSlot.width, (float)GetScreenHeight()-Textures::TextureSlot.height}, 0.0f, 0.8, WHITE);
    }
    DrawText(std::to_string(p.inventory.inventory[i].amount).c_str(), (float)i*Textures::TextureSlot.width, (float)GetScreenHeight()-Textures::TextureSlot.height, 12, RAYWHITE);
  }
  DrawTexture(Textures::TexturePlayer, 250, 300, WHITE);
}

void Player::update(std::vector<std::vector<Block>> b)
{
    float speed = 3.0f;
    float Jspeed = -15.0f;

    int playerWorldX = this->x + 250;
    int playerWorldY = this->y + 300;
    int xindex = (playerWorldX + 16) / 32;
    int yindex = (playerWorldY) / 32;

    bool contact = false;
    if (inBounds(xindex, 0, b.at(0).size()) && inBounds(yindex, 0, b.size()))
    {
        if (b.at(yindex+1).at(xindex).type != AIR)
        {
            contact = true;
            this->y = ((yindex + 1) * 32) - 300 - 32;
        }
    }

    yindex = (playerWorldY+3) / 32;
    xindex = (playerWorldX+32) / 32;
    if (IsKeyDown(KEY_A))
    {
      if(inBounds(xindex, 0, b.at(0).size()) && inBounds(yindex, 0, b.size()))
      {
        if (b.at(yindex).at(xindex-1).type == AIR) this->x -= speed;
      }
    }
    
    xindex = (playerWorldX) / 32;
    if (IsKeyDown(KEY_D))
    {
      if(inBounds(xindex + 1, 0, b.at(0).size()) && inBounds(yindex, 0, b.size()))
      {
        if (b.at(yindex).at(xindex + 1).type == AIR) this->x += speed;
      }
    }
    
    updateY(contact);
    if (IsKeyPressed(KEY_W) && contact)
    {
      this->vy = Jspeed;
      this->y-=0.1;
    }
    yindex = (playerWorldY) / 32;
    if(b.at(yindex).at(xindex).type != AIR)
    {
      contact = false;
      this->vy = 5;
      this->y += 0.1;
    }
}



int main()
{
  InitWindow(GetScreenWidth(), GetScreenHeight(), "main");
  ToggleFullscreen();
  Textures::LoadAll();
  GameState GS = RUNNING;
  Blocks b;
  b.setup();
  Player ClientPlayer;
  SetTargetFPS(30);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(WHITE);

    b.update(ClientPlayer.x, ClientPlayer.y, ClientPlayer.inventory);
    b.updateBlocks(ClientPlayer.x, ClientPlayer.y);
    ClientPlayer.update(b.blocks);

    b.draw(ClientPlayer.x, ClientPlayer.y);
    drawPlayer(ClientPlayer);
    EndDrawing();
  }
  Textures::UnloadAll();
  return 0;
}