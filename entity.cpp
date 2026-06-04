// entity.cpp
#include "entity.hpp"
#include "block.hpp" // This gives the compiler the full definition of Blocks!

Entity::Entity(EntityType type, int x, int y) {
    this->x = (float)x;
    this->y = (float)y;
    if (type == COCK) {
        this->life = 2;
        this->texture = Textures::TextureCock;
    }
}

void Entity::draw(int px, int py) {
    int Bx = (int)x - px;
    int By = (int)y - py;
    DrawTexture(this->texture, Bx, By, {EBright, EBright, EBright, 255});
}

void Entity::update(Blocks& b)
{
    float speed = 2.0f;
    float Jspeed = -12.0f;

    this->vy += this->G;

    int xindex = (int)this->x / 32;
    int yindex = (int)this->y / 32;

    EBright = b.peek(xindex, yindex).Brightness;
    
    if (this->moveR)
    {
        int xDireita = (int)(this->x - 5) / 32;
        if (!b.peek(xDireita + 1, yindex).isSolid()) 
        {
            this->x += speed;
        }
        else 
        {
            this->jump = true;
            if (b.peek(xDireita + 1, yindex - 1).isSolid()) 
            {
                this->moveR = false;
                this->moveL = true;
            }
        }
    }
    else if (this->moveL)
    {
        int xEsquerda = (int)(this->x + 32) / 32;
        if (!b.peek(xEsquerda - 1, yindex).isSolid())
        {
            this->x -= speed;
        }
        else 
        {
            this->jump = true;
            if (b.peek(xEsquerda - 1, yindex - 1).isSolid()) 
            {
                this->moveL = false;
                this->moveR = true;
            }
        }
    }
    
    yindex = (int)this->y / 32;
    bool contact = false;
    
    if (b.peek((int)(this->x + 4) / 32, yindex + 1).isSolid()) contact = true;
    if (!contact)
    {
        if (b.peek((int)(this->x + 20) / 32, yindex + 1).isSolid()) contact = true;
    }

    if(contact && this->vy > 20)
    {
        damage = true;
        this->life--;
    }
    else damage = false;

    if (contact && this->vy > 0)
    {
        this->vy = 0;
        this->y = (yindex * 32);
    }
    
    int xindexTeto = (int)(this->x + 12) / 32;
    if (b.peek(xindexTeto, yindex - 1).isSolid() && this->vy < 0)
    {
        this->vy = 0;
        this->y = (yindex * 32);
    }

    if (this->jump && contact)
    {
        if(!b.peek(xindexTeto, yindex - 1).isSolid()) 
        {
            this->vy = Jspeed;
        }
    }
    
    this->jump = false;
    this->y += this->vy;
}