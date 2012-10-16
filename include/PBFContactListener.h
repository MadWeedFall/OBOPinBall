#ifndef PBFCONTACTLISTENER_H
#define PBFCONTACTLISTENER_H
#include <Box2D/Box2D.h>

enum States
{
    e_add,
    e_persist,
    e_remove
};

struct ContactPoint
{
    b2Shape *shape1;
    b2Shape *shape2;
    b2Vec2 position;
    b2Vec2 velocity;
    b2Vec2 normal;
    float32 separation;
    float32 friction;
    float32 restitution;
    b2ContactID id;
    int state;
};

const int MAXPOINTSCOUNT=2048;

class PBFContactListener:public b2ContactListener
{
    public:

        ContactPoint Contactbuffer[MAXPOINTSCOUNT];
        int pointscount;

        PBFContactListener();
        virtual ~PBFContactListener();
        void Add(const b2ContactPoint *point);
        void Persist(const b2ContactPoint *point);
        void Remove(const b2ContactPoint *point);
        void Result(const b2ContactPoint *point);
    protected:
    private:
};

#endif // PBFCONTACTLISTENER_H
