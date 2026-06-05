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
  if (p.damage)
  {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RED);
  }
  if(p.layer == 'b') DrawText("Current Layer: Back Layer", 16, 16, 16, BLACK); else DrawText("Current Layer: Front Layer", 16, 16, 16, BLACK);
  DrawRectangle(GetScreenWidth() / 2, (GetScreenHeight() / 2)-2, 32, 5, GRAY);
  DrawRectangle(GetScreenWidth() / 2, (GetScreenHeight() / 2)-2, p.life*32/p.max_life, 5, RED);
}

void drawEntities(Blocks& b, Player& p)
{
  int camX = p.x - (GetScreenWidth() / 2);
  int camY = p.y - (GetScreenHeight() / 2);
  for (auto& e : b.Entities)
  {
    e.draw(camX, camY);
  }
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
  ToggleFullscreen();
  Textures::LoadAll();
  GameState GS = MENU;
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
  b.setup();
  while (GS != NOT_RUNING)
  {
    if(WindowShouldClose()) GS = NOT_RUNING;
    
    BeginDrawing();
    if (GS == RUNNING)
    {
      ClearBackground({66, 162, 214, 255});
      time++;
      int camX = ClientPlayer.x - (GetScreenWidth() / 2);
      int camY = ClientPlayer.y - (GetScreenHeight() / 2);
      
      int subsoloY = 3200 - camY;
      int alturaRetangulo = GetScreenHeight() - subsoloY;

      if (subsoloY < GetScreenHeight()) {
          DrawRectangle(0, max(0, subsoloY), GetScreenWidth(), max(0, alturaRetangulo), {104, 61, 40, 255});
      }

      b.update(ClientPlayer.layer, ClientPlayer.x, ClientPlayer.y, ClientPlayer.inventory, time, ClientPlayer.life);
      b.updateBlocks(camX, camY);
      ClientPlayer.update(b, pb);

      b.draw(camX, camY);
      drawEntities(b, ClientPlayer);
      drawPlayer(ClientPlayer, pb);
      if(IsKeyPressed(KEY_P)) GS = PAUSED;
    }
    else if(GS == PAUSED)
    {
      DrawTexture(Textures::TexturePause, 100, 100, {255, 255, 255, 200});
      if(IsKeyPressed(KEY_P)) GS = RUNNING;
    }
    else if (GS == MENU)
    {
      ClearBackground({66, 162, 214, 255});
      DrawTextEx(GetFontDefault(), "Eartharia", (Vector2){(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}, 32, 5, GREEN);
      DrawTextEx(GetFontDefault(), "Press 's' to start", {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2+32}, 16, 5, DARKGREEN);
      if(IsKeyPressed(KEY_S)) GS = RUNNING;
    }
    DrawFPS(10, 30);
    EndDrawing();
  }

  Textures::UnloadAll();
  CloseWindow();
  return 0;
}