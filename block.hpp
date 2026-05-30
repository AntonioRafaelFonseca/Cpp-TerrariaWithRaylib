#pragma once
#include <vector>
#include <iostream>
#include <iterator>
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
              vls.push_back(Block {.type = BlockType::AIR, .LightSource = LightSource::NATURAL});
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
              blocks.at(y).at(x).LightSource= LightSource::NOLIGHT;
            }
            else
            {
              blocks.at(y).at(x).type = BlockType::GRASS;
              blocks.at(y).at(x).LightSource= LightSource::NOLIGHT;
            }
          }
      }
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
      generateTrees();
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
        else if (blocks.at(y).at(x).type == LEAF)
        {
          DrawTexture(Textures::TextureLeafs, Bx, By, {b, b, b, 255});
        }
        else if (blocks.at(y).at(x).type == BARK)
        {
          DrawTexture(Textures::TextureBark, Bx, By, {b, b, b, 255});
        }
        else
        {
          std::cerr << "invalid type at blocks[" << y << "][" << x << "]" << "type = " << blocks.at(y).at(x).type;
          exit(1);
        }
        if (blocks.at(y).at(x).LightSource == TORCH)
        {
          DrawTexture(Textures::TextureTorch, Bx+4, By+4, WHITE);
        }
        else if (blocks.at(y).at(x).LightSource != NOLIGHT && blocks.at(y).at(x).LightSource != NATURAL)
        {
          std::cerr << "invalid Ligth Src at blocks[" << y << "][" << x << "]" << "type = " << blocks.at(y).at(x).LightSource;
          exit(1);
        }
      }
    }
  }

  void update(int px, int py, Inventory& inv)
  {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    bool spacePressed = IsKeyDown(KEY_SPACE);

    bool mousePressedLeft = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool mousePressedRight = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

    if(mousePressedLeft)
    {
      int xindex = (int)(mouseX+px) / 32;
      int yindex = (int)(mouseY+py) / 32;

      if (spacePressed)
      {
        if(blocks.at(yindex).at(xindex).type == AIR)
        {
          std::cout << "blocks line 150";
          blocks.at(yindex).at(xindex).LightSource = TORCH;
        }
      }

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
  void updateBlocks(int px=0, int py=0)
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
    int width = blocks.at(0).size();
    int height = blocks.size();

    int surr[8][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}};
    for (int y=0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        int n = 0;
        int sum = 0;
        int maxB = 0;
        bool foundL = false;
        for(auto s : surr)
        {
          if(!inbounds(x+s[0], 0, width) || !inbounds(y+s[1], 0, height)) continue;

          if(blocks.at(y+s[1]).at(x+s[0]).LightSource == NATURAL)
          {
            blocks.at(y).at(x).Brightness = 255;
            foundL = true;
            break;
          }
          else
          {
            if(blocks.at(y+s[1]).at(x+s[0]).Brightness > maxB)
            {
              maxB = blocks.at(y+s[1]).at(x+s[0]).Brightness;
            }
            sum += blocks.at(y+s[1]).at(x+s[0]).Brightness;
            n++;
          }
        }
        
        if(!foundL) blocks.at(y).at(x).Brightness = (unsigned char)(maxB-std::min(255, sum/n));
      }
    }
  }

  int getInventoryIndexByType(BlockType type, Inventory inventory)
  {
    for (size_t i=0; i<inventory.size; i++)
    {
      if(inventory.at(i).Item.type == type && !inventory.inventory[i].full) return i;
    }
    for (size_t i=0; i<inventory.size; i++)
    {
      if(inventory.at(i).Item.type == BlockType::NONE) return i;
    }
    return 255;
  }
  void generateTrees()
  {
    std::vector<std::vector<int>> pos = getTopLayerXY();
    int tree1[49] = {
      0, 0, 0, 2, 0, 0, 0,
      0, 0, 2, 2, 2, 0, 0,
      0, 2, 2, 2, 2, 2, 0,
      2, 2, 2, 1, 2, 2, 2,
      0, 2, 2, 1, 2, 2, 0,
      0, 0, 0, 1, 0, 0, 0,
      0, 0, 0, 1, 0, 0, 0
    };

    for(auto p : pos)
    {
      
      if(rand() % 25 != 0) continue;
      int grassX = p[0];
      int grassY = p[1];

      for (int i = 0; i < 49; i++)
      {
        int v = tree1[i];
        if(v == 0) continue;

        int xadd = i%7;
        int yadd = i/7;
        
        int factorx = 3;
        int factory = 6;

        int worldX = grassX-factorx+xadd;
        int worldY = grassY-factory+yadd;
        
        if(inbounds(worldY, 0, blocks.size()) && inbounds(worldX, 0, blocks[0].size()))
        {
          if(v == 1) blocks.at(worldY).at(worldX).type = BlockType::BARK;
          if(v == 2) blocks.at(worldY).at(worldX).type = BlockType::LEAF;
        }
      }
    }
  }
  std::vector<std::vector<int>> getTopLayerXY()
  {
    updateBlocks();
    std::vector<std::vector<int>> pos;
    for(int y=0;y<blocks.size()-1;y++)
    {
      for(int x=0;x<blocks.at(y).size()-1;x++)
      {
        int Bx = x*32;
        int By = y*32;

        if (blocks.at(y).at(x).type == AIR)
        {
          continue;
        }
        else
        {
          if(blocks.at(y-1).at(x).type == AIR) pos.push_back({x, y});
        }
      }
    }
    return pos;
  }
  std::vector<std::vector<int>> getBelowX(int x)
  {
    std::vector<std::vector<int>> top = getTopLayerXY();
    std::vector<std::vector<int>> VecBlocks;
    for(int i=0;i<blocks.size();i++){
      for(int j=0;j<blocks.size();j++){
        for(auto block : top)
        {
          if(i<block[1] && j == block[0])
          {
            VecBlocks.push_back({j, i});
          }
        }
      }
    }
    return VecBlocks;
  }
};