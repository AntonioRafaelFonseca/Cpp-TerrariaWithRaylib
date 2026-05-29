#pragma once
#include <vector>
#include <iostream>
#include "raylib.h"
#include "perlin.hpp"
#include "inventory.hpp"
#include "block_type.hpp"
#include "textures.hpp"
bool inbounds(int n, int min, int max)
{
  if(n<max && n>=min) return true;
  return false;
}

class Blocks
{
  public:
  
  std::vector<std::vector<Block>> blocks;
  void setup()
  {
      int width = 500;
      int height = 60;
      // 1. Inicializa primeiro todo o mapa como AIR
      for(int y = 0; y < height; y++)
      {
          std::vector<Block> vls;
          for (int x = 0; x < width; x++)
          {
              vls.push_back(Block {.type = BlockType::AIR});
          }
          blocks.push_back(vls);
      }
  
      // 2. Aplica o Perlin Noise para gerar a superfície do terreno
      for(int x = 0; x < width; x++)
      {
          float noiseValue = perlin(x / 15.0f, 0.0f);
          float normalizedNoise = (noiseValue + 1.0f) / 2.0f;
          int baseLine = height/2;  // Altura média do mundo
          int amplitude = (int)25; // Altura máxima das montanhas
          int surfaceY = baseLine + (int)(normalizedNoise * amplitude);
          
          if (surfaceY < 0) surfaceY = 0;
          if (surfaceY >= 100) surfaceY = 99;
  
          for (int y = surfaceY; y < height; y++)
          {
            if(rand()%50 == 0)
            {
              blocks.at(y).at(x).type = BlockType::STONE;
            }
            else
            {
              blocks.at(y).at(x).type = BlockType::GRASS;
            }
          }
      }
      updateBlocks(0, 0);
      int surr[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
      for(int y=0;y<height;y++)
      {
        for(int x=0;x<width;x++)
        {
          if(blocks.at(y).at(x).type == BlockType::STONE)
          {
            for(auto s : surr)
            {
              if(!inbounds(y+s[1], 0, height) || !inbounds(x+s[0], 0, width)) continue;
              if(rand() % 3 == 0) blocks.at(y+s[1]).at(x+s[0]).type = STONE;
            }
          }
        }
      }
    updateBrightness();
  }
  void draw(int px, int py)
  {
    for(int y=0;y<blocks.size()-1;y++)
    {
      for(int x=0;x<blocks.at(y).size()-1;x++)
      {
        int Bx = x*32-px;
        int By = y*32-py;
        if (Bx < -32 || Bx > GetScreenWidth() || By < -32 || By > GetScreenHeight()) continue;
        unsigned char b = blocks.at(y).at(x).Brightness;
        if (blocks.at(y).at(x).type == AIR)
        {
          DrawTexture(Textures::TextureAir, Bx, By, {b, b, b, 255});
        }
        else if (blocks.at(y).at(x).type == GRASS)
        {
          DrawTexture(Textures::TextureGrass, Bx, By, {b, b, b, 255});
        }
        else if (blocks.at(y).at(x).type == DIRT)
        {
          DrawTexture(Textures::TextureDirt, Bx, By, {b, b, b, 255});
        }
        else if (blocks.at(y).at(x).type == STONE)
        {
          DrawTexture(Textures::TextureStone, Bx, By, {b, b, b, 255});
        }
        else
        {
          std::cerr << "invalid type at blocks[" << y << "][" << x << "]" << "type = " << blocks.at(y).at(x).type;
          exit(1);
        }
      }
    }
  }
  void update(int px, int py, Inventory& inv)
  {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    bool mousePressedLeft = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool mousePressedRight = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

    if(mousePressedLeft)
    {
      int xindex = (int)(mouseX+px) / 32;
      int yindex = (int)(mouseY+py) / 32;

      try
      {
        if(blocks.at(yindex).at(xindex).type == AIR && inv.inventory[inv.selected].Item.type != NONE)
        {
          blocks.at(yindex).at(xindex).type = inv.inventory[inv.selected].Item.type;
          inv.inventory[inv.selected].amount -= 1;
        }
      }
      catch(std::out_of_range)
      {
        std::cout << "\nx: " << xindex << "\ny: " << yindex << "\n\n";
      }
      updateInventory(inv);
      updateBrightness();
    }
    if(mousePressedRight)
    {
      int xindex = (int)(mouseX+px) / 32;
      int yindex = (int)(mouseY+py) / 32;
      
      if (blocks.at(yindex).at(xindex).type != AIR)
      {
        try
        {
          int inventoryIndex = getInventoryIndexByType(blocks.at(yindex).at(xindex).type, inv);
          if(inventoryIndex != 255)
          {
            if(inv.inventory[inventoryIndex].Item.type != blocks.at(yindex).at(xindex).type)
            {
              inv.inventory[inventoryIndex].Item.type = blocks.at(yindex).at(xindex).type;
            }
            inv.inventory[inventoryIndex].amount += 1;
          }
          blocks.at(yindex).at(xindex).type = AIR;
        }
        catch(std::out_of_range)
        {
          std::cout << "\nx: " << xindex << "\ny: " << yindex << "\n\n";
        }
        updateInventory(inv);
        updateBrightness();
      }
    }
    if(IsKeyPressed(KEY_LEFT)) inv.selected--;
    if(IsKeyPressed(KEY_RIGHT)) inv.selected++;
  }
  void updateBlocks(int px, int py)
  {
    for(int y=0;y<blocks.size()-1;y++)
    {
    for(int x=0;x<blocks.at(y).size()-1;x++)
    {
      int Bx = x*32-px;
      int By = y*32-py;
      if (Bx < -32 || Bx > GetScreenWidth() || By < -32 || By > GetScreenHeight()) continue;

      if (blocks.at(y).at(x).type == AIR)
      {
        continue;
      }
      else if (blocks.at(y).at(x).type == GRASS)
      {
        if(blocks.at(y-1).at(x).type != AIR) blocks.at(y).at(x).type = DIRT;
      }

    }
  }
  }
  void updateBrightness()
  {
    int surr[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    for(int i = 0;i<blocks.size();i++)
      {
        for(int j = 0;j<blocks.at(0).size();j++)
        {
          if (blocks.at(i).at(j).type == AIR)
          {
            blocks.at(i).at(j).Brightness = 255;
            for(int a=0;a<8;a++)
            {
              if(!inbounds(surr[a][0]+i, 0, blocks.size()) || !inbounds(surr[a][1]+j, 0, blocks.at(0).size())) continue;
              if(blocks.at(surr[a][0]+i).at(surr[a][1]+j).type == AIR) continue;
              blocks.at(surr[a][0]+i).at(surr[a][1]+j).Brightness = 255;
            }
          }
        }
      }
  }
  int getInventoryIndexByType(BlockType type, Inventory inventory)
  {
    for (int i=0;i<std::size(inventory.inventory);i++)
    {
      if(inventory.at(i).Item.type == type && !inventory.inventory[i].full) return i;
    }
    for (int i=0;i<std::size(inventory.inventory);i++)
    {
      if(inventory.at(i).Item.type == BlockType::NONE) return i;
    }
    return 255;
  }
};