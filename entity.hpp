// entity.hpp
#pragma once
#include "raylib.h"
#include "textures.hpp"

class Blocks; // This forward declaration is still perfect here!

enum EntityType : unsigned char { COCK };

class Entity {
public:
    EntityType type;
    float x; float y; float vy = 0; float G = 1.0f;
    int life; bool damage = false; unsigned char EBright = 255;
    bool moveL = false; bool moveR = true; bool jump = false;
    Texture2D texture;

    Entity(EntityType type, int x, int y);
    void draw(int px, int py);
    
    // ONLY the signature here. Remove the curly braces and all the logic.
    void update(Blocks& b); 
};