#include "../include/Entity.h"

bool Entity::LoadSprite(HTEXTURE tex,float x,float y,float w,float h)
{
    sprite = new hgeSprite(tex,x,y,w,h);
    sprite->SetHotSpot(sprite->GetWidth()/2,sprite->GetHeight()/2);
    if(sprite==NULL)
    {
        return false;
    }
    return true;
}

void Entity::Render()
{
    if(sprite!=NULL)
        sprite->RenderEx(x,y,rotation,1.0,1.0);
}

Entity::Entity()
{
    rotation=0.0;
    id=0;
    x=0;
    y=0;
    vx=0;
    vy=0;
    //ctor
}

Entity::~Entity()
{
    delete sprite;
    //dtor
}
