#ifndef ENTITY_H
#define ENTITY_H
#include <HGE/hge.h>
#include <HGE/hgesprite.h>
#include <Box2D/Box2D.h>

class Entity
{
public:
    hgeSprite* sprite;//精灵对象
    b2Body* body;//游戏角色对应的物理实体
    hgeQuad quad;//实体对应的四边形贴图,是最基本的显示图像的方式,一般用不到
    HEFFECT sound;//对应于实体碰撞等动作的声音
    float x,y;//实体的位置
    float rotation;
    float vx,vy,v;//实体在x,y方向上的速度，实体速度的标量
    int id;

    bool LoadSprite(HTEXTURE tex,float x,float y,float w,float h);
    void Render();

    Entity();
    virtual ~Entity();
protected:
private:


};

#endif // ENTITY_H
