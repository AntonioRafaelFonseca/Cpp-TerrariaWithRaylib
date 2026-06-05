// entity.cpp
#include "entity.hpp"
#include "block.hpp" 
#include <cmath>     // Required for floorf

Entity::Entity(EntityType type, int x, int y) {
    this->x = (float)x;
    this->y = (float)y;
    if (type == COCK) {
        this->life = 2;
        this->max_life = 2;
        this->texture = Textures::TextureCock;
    }
}

void Entity::draw(int px, int py) {
    int Bx = (int)x - px;
    int By = (int)y - py;
    if(damage) DrawRectangle(Bx, By, 32, 32, RED);
    else DrawTexture(this->texture, Bx, By, {EBright, EBright, EBright, 255});
    DrawRectangle(Bx, By-2, 32, 5, GRAY);
    DrawRectangle(Bx, By-2, this->life*32/this->max_life, 5, RED);
}

void Entity::update(Blocks& b)
{
    // 1. Random AI Wandering
    if(rand() % 300 == 0 && !b.call)
    {
        if(rand() % 2 == 0) {
            moveL = true;
            moveR = false;
        } else {
            moveL = false;
            moveR = true;
        }
    }


    float speed = 1.5f;     // Slightly slower than player so it doesn't zip around uncontrollably
    float Jspeed = -10.5f;

    // Apply gravity
    this->vy += this->G;
    if (this->vy > 16.0f) this->vy = 16.0f; // Terminal velocity cap to prevent clipping

    // Safely calculate current tile indices using floorf
    int xindex = (int)floorf(this->x / 32.0f);
    int yindex = (int)floorf(this->y / 32.0f);

    EBright = b.peek(xindex, yindex).Brightness;
    
    // 2. Horizontal Movement & Wall Detection
    // 2. Horizontal Movement & Wall Detection
    if (this->moveR)
    {
        int nextXRight = (int)floorf((this->x + 32.0f + speed) / 32.0f);
        BlockType blockAhead = b.peek(nextXRight, yindex).type;
        
        if (!b.peek(nextXRight, yindex).isSolid() && blockAhead != NORMALFENCE)
        {
            this->x += speed;
        }
        else // Obstacle detected
        {
            // ONLY jump if the obstacle isn't a fence!
            if (blockAhead != NORMALFENCE) 
            {
                this->jump = true;
            }
            
            // If the wall is 2 blocks high, turn around instead
            if (b.peek(nextXRight, yindex - 1).isSolid())
            {
                this->moveR = false;
                this->moveL = true;
            }
        }
    }
    else if (this->moveL)
    {
        int nextXLeft = (int)floorf((this->x - speed) / 32.0f);
        BlockType blockAhead = b.peek(nextXLeft, yindex).type;
        
        if (!b.peek(nextXLeft, yindex).isSolid() && blockAhead != NORMALFENCE)
        {
            this->x -= speed;
        }
        else // Obstacle detected
        {
            // ONLY jump if the obstacle isn't a fence!
            if (blockAhead != NORMALFENCE) 
            {
                this->jump = true;
            }
            
            // If the wall is 2 blocks high, turn around instead
            if (b.peek(nextXLeft, yindex - 1).isSolid())
            {
                this->moveL = false;
                this->moveR = true;
            }
        }
    }

    // Recalculate Y index after potential X updates
    yindex = (int)floorf(this->y / 32.0f);
    bool contact = false;
    
    // 3. Vertical Ground Collision (Check left leg and right leg offsets)
    int footY = (int)floorf((this->y + 32.0f + this->vy) / 32.0f);
    int leftFootX = (int)floorf((this->x + 4.0f) / 32.0f);
    int rightFootX = (int)floorf((this->x + 28.0f) / 32.0f);

    if (b.peek(leftFootX, footY).isSolid() || b.peek(rightFootX, footY).isSolid()) 
    {
        contact = true;
    }

    // Fall Damage Check
    if(contact && this->vy > 13.0f)
    {
        damage = true;
        this->life--;
    }
    else damage = false;

    // Land on ground safely
    if (contact && this->vy > 0)
    {
        this->vy = 0;
        this->y = (float)(footY - 1) * 32.0f;
    }
    
    // 4. Ceiling Collision Checks
    int headY = (int)floorf((this->y + this->vy) / 32.0f);
    int centerHeadX = (int)floorf((this->x + 16.0f) / 32.0f);
    
    if (b.peek(centerHeadX, headY).isSolid() && this->vy < 0)
    {
        this->vy = 0;
    }

    int currentGridX = (int)floorf(this->x / 32.0f);
    int currentGridY = (int)floorf(this->y / 32.0f);

    // 2. Safely look 1 grid tile ahead depending on direction
    BlockType targetBlocksTypeLeftRight = (moveL) ? b.peek(currentGridX - 1, currentGridY).type
                                                : b.peek(currentGridX + 1, currentGridY).type;
    if (this->jump && contact)
    {
        // Only jump if there isn't a block directly overhead blocking them
        if(!b.peek(centerHeadX, yindex - 1).isSolid() && targetBlocksTypeLeftRight != NORMALFENCE)
        {
            this->vy = Jspeed;
        }
    }
    
    this->jump = false;
    this->y += this->vy;
}

void Entity::moveTowards(int x)
{
    if (this->x > x)
    {
        moveL = true;
        moveR = false;
    }
    else if (this->x < x)
    {
        moveL = false;
        moveR = true;
    }
}