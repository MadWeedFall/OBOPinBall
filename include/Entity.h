#ifndef ENTITY_H
#define ENTITY_H
#include <HGE/hge.h>
#include <HGE/hgesprite.h>
#include <Box2D/Box2D.h>

class Entity
{
public:
    hgeSprite* sprite;//�������
    b2Body* body;//��Ϸ��ɫ��Ӧ������ʵ��
    hgeQuad quad;//ʵ���Ӧ���ı�����ͼ,�����������ʾͼ��ķ�ʽ,һ���ò���
    HEFFECT sound;//��Ӧ��ʵ����ײ�ȶ���������
    float x,y;//ʵ���λ��
    float rotation;
    float vx,vy,v;//ʵ����x,y�����ϵ��ٶȣ�ʵ���ٶȵı���
    int id;

    bool LoadSprite(HTEXTURE tex,float x,float y,float w,float h);
    void Render();

    Entity();
    virtual ~Entity();
protected:
private:


};

#endif // ENTITY_H
