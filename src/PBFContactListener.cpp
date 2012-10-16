#include "../include/PBFContactListener.h"

PBFContactListener::PBFContactListener()
{
    pointscount=0;
}

PBFContactListener::~PBFContactListener()
{

}

//实现ADD方法，该方法在碰撞刚刚发生时触发
void PBFContactListener::Add(const b2ContactPoint *point)
{
    ContactPoint *cp = Contactbuffer+pointscount;

    cp->shape1=point->shape1;
    cp->shape2=point->shape2;
    cp->normal=point->normal;
    cp->velocity=point->velocity;
    cp->position=point->position;
    cp->id=point->id;
    cp->restitution=point->restitution;
    cp->separation=point->separation;
    cp->state=e_add;

    ++pointscount;
}

//实现Persist,该方法在碰撞刚体保持接触这段时间触发
void PBFContactListener::Persist(const b2ContactPoint *point)
{
    /*
    ContactPoint *cp = Contactbuffer+pointscount;

    cp->shape1=point->shape1;
    cp->shape2=point->shape2;
    cp->normal=point->normal;
    cp->velocity=point->velocity;
    cp->position=point->position;
    cp->id=point->id;
    cp->restitution=point->restitution;
    cp->separation=point->separation;
    cp->state=e_persist;

    ++pointscount;
    */
}

//实现Remove方法，该方法在碰撞物体刚刚分离时触发
void PBFContactListener::Remove(const b2ContactPoint *point)
{
    ContactPoint *cp = Contactbuffer+pointscount;

    cp->shape1=point->shape1;
    cp->shape2=point->shape2;
    cp->normal=point->normal;
    cp->velocity=point->velocity;
    cp->position=point->position;
    cp->id=point->id;
    cp->restitution=point->restitution;
    cp->separation=point->separation;
    cp->state=e_remove;

    ++pointscount;
}

void PBFContactListener::Result(const b2ContactPoint *point)
{

}
