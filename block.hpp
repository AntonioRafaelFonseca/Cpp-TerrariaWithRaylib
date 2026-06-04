#pragma once
#include <queue>
#include <vector>
#include <iostream>
#include <iterator>
#include <print>
#include "raylib.h"
#include "perlin.hpp"
#include "inventory.hpp"
#include "block_type.hpp"
#include "textures.hpp"
#include "crafter.hpp"

bool inbounds(int n, int min, int max)
{
  if(n<max && n>=min) return true;
  return false;
}

class Blocks
{
  public:
  Crafter crafter;
  std::vector<Block> blocks;
  std::vector<SmallBlock> BackLayerBlocks;
  Block  unusedBlock {.type = BlockType::NONE};
  SmallBlock  unusedBBlock {.type = BlockType::NONE};
  int width = 1000;
  int height = 200;
  bool loading = true;
  void setup()
  {
    loading = true;
    blocks.assign(width * height, Block {.type = BlockType::AIR, .LightSource = LightSourceType::NATURAL});
    BackLayerBlocks.assign(width*height, SmallBlock());
      for(int x = 0; x < width; x++)
        {
            float noise1 = perlin(x / 80.0f, 0.0f);

            float noise2 = perlin(x / 12.0f, 0.0f);
            float combinado = (noise1 * 0.8f) + (noise2 * 0.2f);
            float normalizedNoise = (combinado + 1.0f) / 2.0f;
            int baseLine = height / 2;
            int amplitude = 60;
            int surfaceY = baseLine + (int)(normalizedNoise * amplitude);
            if (surfaceY < 0) surfaceY = 0;
            if (surfaceY >= height) surfaceY = height - 1;
            for (int y = surfaceY; y < height; y++)
            {
                if(rand() % 50 == 0)
                {
                  if (rand()%2 == 0)
                  {
                    peek(x, y).type = BlockType::STONE;
                  }
                  else
                  {
                    peek(x, y).type = BlockType::COAL;
                  }
                    peek(x, y).LightSource = LightSourceType::NOLIGHT;
                }
                
                else
                {
                    peek(x, y).type = BlockType::GRASS;
                    peek(x, y).LightSource = LightSourceType::NOLIGHT;
                }
            }
        }
      int surr[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
      for(int y=0;y<height;y++)
      {
        for(int x=0;x<width;x++)
        {
          if(peek(x, y).type == BlockType::STONE)
          {
            for(auto s : surr)
            {
              if(!validPos(x+s[0], y+s[1])) continue;
              if(rand() % 3 == 0) peek(x+s[0], y+s[1]).type = STONE;
            }
          }
        }
      }
      generateTrees();
    updateBrightness();
    loading = false;
  }
  void draw(int px, int py)
  {
    DrawTextureEx(Textures::TextureBackground, {(float)0-px/2, (float)700-py/2}, 0.0f, 3, {170, 170, 245, 255});
    for(int y=0;y<getHeight();y++)
    {
      for(int x=0;x<getWidth();x++)
      {
        int Bx = x*32-px;
        int By = y*32-py;
        if (Bx < -32 || Bx > GetScreenWidth() || By < -32 || By > GetScreenHeight()) continue;
        unsigned char b = peek(x, y).Brightness;
        if(peekBackLayer(x, y).type != AIR)
        {
          DrawTexture(getTextureByType(peekBackLayer(x, y).type), Bx, By, {(unsigned char)(b-50), (unsigned char)(b-50), (unsigned char)(b-50), (unsigned char)(255)});
        }
        if(peek(x, y).type == AIR) {
          DrawRectangle(Bx, By, 32, 32, {0, 0, 0, (unsigned char)(255-b)});
        }
        else if(peek(x, y).type != AIR)
        {
          DrawTexture(getTextureByType(peek(x, y).type), Bx, By, {b, b, b, 255});
        }
        else if (peek(x, y).type != NONE)
        {
          std::cerr << "line 121 block.hpp invalid type at blocks[" << y << "][" << x << "]" << "type = " << peek(x, y).type;
          exit(1);
        }
      }
    }
  }

  void update(LayerIndex li, int px, int py, Inventory& inv, int& time)
{
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    bool spacePressed = IsKeyDown(KEY_SPACE);
    bool interacting = IsKeyDown(KEY_S);

    bool mousePressedLeft = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool mousePressedRight = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

    int xindex = (int)(mouseX + px) / 32;
    int yindex = (int)(mouseY + py) / 32;

    // 1. CHECK CRAFTING FIRST (Space + Left Click)
    if (mousePressedLeft && spacePressed)
    {
        if (validPos(xindex, yindex))
        {
            BlockType targetedType = (li == 'b') ? peekBackLayer(xindex, yindex).type : peek(xindex, yindex).type;
            
            if (targetedType == CRAFTER)
            {
                returnerValue craftedItem = crafter.craft();
                if (craftedItem.type != NONE)
                {
                    int invIndex = getInventoryIndexByType(craftedItem.type, inv);
                    if (invIndex != 255)
                    {
                        inv.inventory[invIndex].Item.type = craftedItem.type;
                        inv.inventory[invIndex].amount += craftedItem.amount;
                        std::cout << "Successfully crafted and added to inventory!\n";
                    }
                }
                else
                {
                    std::cout << "Recipe does not match: \n";
                    for (auto b : crafter.blocks)
                    {
                      std::cout << b;
                    }
                }
                updateInventory(inv);
                return;
            }
        }
    }

    // 2. INTERACTING MODE (Holding S key) - Add items to Crafter
    if (interacting)
    {
        if (mousePressedLeft)
        {
            int xInventoryindex = mouseX / Textures::TextureSlot.width;
            
            // Selecting slot in UI
            if (inbounds(xInventoryindex, 0, 5) &&
                inbounds(mouseY, GetScreenHeight() - Textures::TextureSlot.height, GetScreenHeight()))
            {
                inv.selected = xInventoryindex;
            }
            // Clicking world object with UI item selected
            else if (validPos(xindex, yindex) && inv.selected <= 5)
            {
                BlockType targetedType = (li == 'b') ? peekBackLayer(xindex, yindex).type : peek(xindex, yindex).type;
                
                if (targetedType == CRAFTER && inv.inventory[inv.selected].amount > 0)
                {
                  if(time % 15 == 0)
                  {
                      time = 0;
                      crafter.add(inv.inventory[inv.selected].Item.type);
                      inv.inventory[inv.selected].amount--;
                      updateInventory(inv);
                      time++;
                    }
                }
            }
        }
    }
    else
    {
        if (mousePressedLeft)
        {
            if (validPos(xindex, yindex))
            {
                if ((peek(xindex, yindex).type == AIR && li == 'f') || (peekBackLayer(xindex, yindex).type == AIR && li == 'b'))
                {
                    if (inv.inventory[inv.selected].Item.type != NONE)
                    {
                        if (li == 'f')      peek(xindex, yindex).type = inv.inventory[inv.selected].Item.type;
                        else if (li == 'b') peekBackLayer(xindex, yindex).type = inv.inventory[inv.selected].Item.type;
                        
                        inv.inventory[inv.selected].amount -= 1;
                        QUICKupdateBrightness(px, py);
                      }
                }
                updateInventory(inv);
            }
        }
        if (mousePressedRight)
        {
            if (validPos(xindex, yindex))
            {
                Block& targetBlock = (li == 'f') ? peek(xindex, yindex) : (Block&)peekBackLayer(xindex, yindex);
                
                if (targetBlock.type != AIR)
                {
                    int inventoryIndex = getInventoryIndexByType(targetBlock.type, inv);
                    if (inventoryIndex != 255)
                    {
                        inv.inventory[inventoryIndex].Item.type = targetBlock.type;
                        inv.inventory[inventoryIndex].amount += 1;
                    }
                    QUICKupdateBrightness(px, py);
                    targetBlock.type = AIR;
                }
                updateInventory(inv);
            }
        }
    }
    
    if (IsKeyPressed(KEY_LEFT))  inv.selected--;
    if (IsKeyPressed(KEY_RIGHT)) inv.selected++;
}
  void updateBlocks(int px=0, int py=0)
  {
    for(int y=0;y<getHeight()-1;y++)
    {
    for(int x=0;x<getWidth()-1;x++)
    {
      int Bx = x*32-px;
      int By = y*32-py;
      if (Bx < -32 || Bx > GetScreenWidth() || By < -32 || By > GetScreenHeight()) continue;

      if (peek(x, y).type == AIR)
      {
        continue;
      }
      else if (peek(x, y).type == GRASS)
      {
        if(!validPos(0, y-1)) continue;
        if(peek(x, y-1).type != AIR) peek(x, y).type = DIRT;
      }
      else if(peek(x, y).type == TORCH)
      {
        peek(x, y).LightSource == TYPETORCH;
      }
    }
  }
  }
  void QUICKupdateBrightness(int Bx, int By)
{
    // Calculate tile boundaries currently visible on the screen
    int startX = std::max(0, Bx / 32 - 2);
    int endX = std::min(width, (Bx + GetScreenWidth()) / 32 + 2);
    int startY = std::max(0, By / 32 - 2);
    int endY = std::min(height, (By + GetScreenHeight()) / 32 + 2);

    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    // Only update tiles within the camera view
    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            if (peek(x, y).LightSource == LightSourceType::NATURAL || peek(x, y).type == BlockType::TORCH)
            {
                peek(x, y).Brightness = 255;
                continue;
            }

            int maxB = 25; // Default minimum darkness
            for (auto& d : directions)
            {
                int nx = x + d[0];
                int ny = y + d[1];
                if (!validPos(nx, ny)) continue;

                int loss = peek(x, y).isSolid() ? 45 : 20;
                int candidate = (int)peek(nx, ny).Brightness - loss;
                if (candidate > maxB) maxB = candidate;
            }
            peek(x, y).Brightness = (unsigned char)maxB;
        }
    }
}
  void updateBrightness()
  {
      std::queue<std::pair<int, int>> lightQueue;
  
      // Step 1: Reset all non-natural light to minimum darkness, 
      // and find all starting light sources (Sky/Torches)
      for (int y = 0; y < height; y++)
      {
          for (int x = 0; x < width; x++)
          {
              if (peek(x, y).LightSource == LightSourceType::NATURAL || peek(x, y).type == BlockType::TORCH)
              {
                  peek(x, y).Brightness = 255;
                  lightQueue.push({x, y});
              }
              else
              {
                  peek(x, y).Brightness = 5; // Base minimum darkness
              }
          }
      }
  
      // Step 2: Flood fill outwards
      int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // Up, Down, Left, Right
      int lightDrop = 32; // How much darker the light gets per block step
  
      while (!lightQueue.empty())
      {
          auto [cx, cy] = lightQueue.front();
          lightQueue.pop();
  
          int currentBrightness = peek(cx, cy).Brightness;
  
          for (auto& d : directions)
          {
              int nx = cx + d[0];
              int ny = cy + d[1];
  
              if (!validPos(nx, ny)) continue;
  
              // Solid blocks block light more heavily
              int drop = peek(nx, ny).isSolid() ? (lightDrop * 2) : lightDrop;
              int expectedBrightness = currentBrightness - drop;
  
              if (expectedBrightness < 25) expectedBrightness = 25;
  
              // If the neighboring block is darker than our light wave, illuminate it!
              if (peek(nx, ny).Brightness < expectedBrightness)
              {
                  peek(nx, ny).Brightness = (unsigned char)expectedBrightness;
                  lightQueue.push({nx, ny}); // Push neighbor to keep flooding
              }
          }
      }
  }
  int getInventoryIndexByType(BlockType type, Inventory& inventory)
  {
    if(type == NONE) return 255;
    for (size_t i=0; i<inventory.size; i++)
    {
      if(inventory.at(i).Item.type == type && inventory.inventory[i].amount != 0) return i;
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
        
        if(validPos(worldX, worldY))
        {
          if(v == 1) peek(worldX, worldY).type = BlockType::BARK;
          if(v == 2) peek(worldX, worldY).type = BlockType::LEAF;
        }
      }
    }
  }
  std::vector<std::vector<int>> getTopLayerXY()
  {
    updateBlocks();
    std::vector<std::vector<int>> pos;
    for(int y=0;y<getHeight()-1;y++)
    {
      for(int x=0;x<getWidth();x++)
      {
        int Bx = x*32;
        int By = y*32;

        if (peek(x, y).type == AIR)
        {
          continue;
        }
        else
        {
          if(validPos(x, y-1)) if(peek(x, y-1).type == AIR) pos.push_back({x, y});
        }
      }
    }
    return pos;
  }
  std::vector<std::vector<int>> getBelowX(int x)
  {
    std::vector<std::vector<int>> top = getTopLayerXY();
    std::vector<std::vector<int>> VecBlocks;
    for(int i=0;i<getHeight();i++){
      for(int j=0;j<getHeight();j++){
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
  Block& peek(int posx, int posy)
  {
    int index = posy*getWidth()+posx;
    if(validPos(posx, posy)) return blocks.at(index);
    return this->unusedBlock;
  }
  SmallBlock& peekBackLayer(int posx, int posy)
  {
    int index = posy*getWidth()+posx;
    if(validPos(posx, posy)) return BackLayerBlocks.at(index);
    return this->unusedBBlock;
  }
  bool validPos(int x, int y)
  {
    return (inbounds(x, 0, getWidth()) && inbounds(y, 0, getHeight()));
  }
  int getWidth()
  {
    return this->width;
  }
  
  int getHeight()
  {
    return this->height;
  }

};