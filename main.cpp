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

void drawPlayer(Player& p, unsigned char pb)
{
  for (int i=0;i<p.life;i++)
  {
    if (i%2==0) DrawTexture(Textures::TextureHeart0, i*32-10, 16, WHITE);
    else DrawTexture(Textures::TextureHeart1, i*32-42, 16, WHITE);
  }
  for(int i=0;i<5;i++)
  {
    DrawTexture(Textures::TextureSlotarrow, p.inventory.selected*32, (float)GetScreenHeight()-64, WHITE);
    if(i==p.inventory.selected)
    {
      DrawTexture(Textures::TextureSlot, i*Textures::TextureSlot.width, GetScreenHeight()-Textures::TextureSlot.height, {205, 205, 205, 255});
    }
    else{
      DrawTexture(Textures::TextureSlot, i*Textures::TextureSlot.width, GetScreenHeight()-Textures::TextureSlot.height, WHITE);
    }
    
    DrawTextureEx(getTextureByType(p.inventory.inventory[i].Item.type),
                  {(float)i*Textures::TextureSlot.width, (float)GetScreenHeight()-Textures::TextureSlot.height-4},
                  0.0f, 0.8, WHITE);
    
    DrawText(std::to_string(p.inventory.inventory[i].amount).c_str(), (float)i*Textures::TextureSlot.width, (float)GetScreenHeight()-Textures::TextureSlot.height, 12, RAYWHITE);
  }
  DrawTexture(Textures::TexturePlayer, GetScreenWidth() / 2, GetScreenHeight() / 2, {pb, pb, pb, 255});
}

int max(int a, int b) {
  if(a>b) return a;
  return b;
}
int min(int a, int b) {
  if(a>b) return b;
  return a;
}


int main()
{
  InitWindow(GetScreenWidth(), GetScreenHeight(), "main");
  // ToggleFullscreen();
  Textures::LoadAll();
  GameState GS = RUNNING;
  Blocks b;
  Player ClientPlayer;
  SetTargetFPS(30);
  unsigned char pb;
  ClientPlayer.inventory.inventory[0].Item.type = CRAFTER;
  ClientPlayer.inventory.inventory[0].amount = 1;
  int avgH = (int)b.getHeight()/2;
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();
  bool started = false;
  int time = 0;
  while (!WindowShouldClose())
  {
    
    BeginDrawing();
    ClearBackground({66, 162, 214, 255});
    if(b.loading)
    {
      if(!started)
      {
        b.setup();
        started = 0b00000001;
      }
      DrawText("Loading...", 100, 100, 25, BLACK);
    }
    else
    {
      time++;
      int camX = ClientPlayer.x - (GetScreenWidth() / 2);
      int camY = ClientPlayer.y - (GetScreenHeight() / 2);
      
      int subsoloY = 3200 - camY;
      int alturaRetangulo = GetScreenHeight() - subsoloY;

      if (subsoloY < GetScreenHeight()) {
          DrawRectangle(0, max(0, subsoloY), GetScreenWidth(), max(0, alturaRetangulo), {104, 61, 40, 255});
      }

      b.update(ClientPlayer.layer, camX, camY, ClientPlayer.inventory, time);
      b.updateBlocks(camX, camY);
      ClientPlayer.update(b, pb);

      b.draw(camX, camY);
      drawPlayer(ClientPlayer, pb);
    }
    DrawFPS(100, 100);
    EndDrawing();
  }
  Textures::UnloadAll();
  return 0;
}