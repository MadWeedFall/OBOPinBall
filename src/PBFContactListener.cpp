#include "../include/PBFContactListener.h"

PBFContactListener::PBFContactListener()
{
    pointscount=0;
}

PBFContactListener::~PBFContactListener()
{

}

//ʵ��ADD�������÷�������ײ�ոշ���ʱ����
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

//ʵ��Persist,�÷�������ײ���屣�ֽӴ����ʱ�䴥��
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

//ʵ��Remove�������÷�������ײ����ոշ���ʱ����
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
