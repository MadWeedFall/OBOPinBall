/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/

#include <iostream>
#include <vector>
#include <HGE/hge.h>
#include <HGE/hgefont.h>
#include <HGE/hgeparticle.h>
#include <Box2D/Box2D.h>
#include "include/Entity.h"
#include "include/PBFContactListener.h"

using namespace std;



const float screenWidth=800.0;
const float screenHeight=600.0;

HGE *hge = NULL;
hgeFont *fnt;//HGE�е�������󣬿�������Ļ��д�֣���֪��֧��֧�����ģ�201294

b2Vec2 gravity(0.0f,-10.0f);
b2AABB border;
bool doSleep=true;
b2World *physc;
float timeStep=1.0/20.0;
int iteration=10;
int score=0;
float dt;//delta,����һ֡����ǰ��ʱ��

//��Ļ�Ͽ����ſ����ŵ����ж��������������ø�����װ���������Ƿ����������棬����ô�Ű�
Entity *ent;//С��Ϊʲô����ball����Ϊ����
Entity *ground;//���棬���ڵ��Ե�ʱ���������������ȥ��
Entity *upbound;
Entity *leftbound;
Entity *bkg;//��Ϊ������ܶණ�����ǹ̶������ģ����Ծ�ֱ�ӻ��ڱ������ˣ���ʵ֤���������鷳
Entity *slideL;//���б��
Entity *slideR;//�Ҳ�б��
Entity *holderL;//�������󵲰�
Entity *holderR;//�������ҵ���
Entity *rollslide;//���Ͻǹ��£������С���ɴ˸ı䷽��
Entity *bumper;//ײ�ӣ�������С��ײ��ȥ
Entity *cursor;//���ָ��
Entity *fliperL;//��൯��
Entity *fliperR;//�Ҳ൯��
vector<Entity*> bingers;//��Ļ�����е�binger

hgeParticleSystem *par;//С���β��������Ȥζ��

b2PrismaticJoint *pj; //����ģ�ⵯ��
b2RevoluteJoint *fjL; //������൯����ת
b2RevoluteJoint *fjR; //�����Ҳ൯����ת

PBFContactListener pbfcontactlistener;

void boom(int x,float v)
{
    int pan=x;
	float pitch=v*0.0005f+0.2f;
	hge->Effect_PlayEx(ent->sound,100,pan,pitch);
}

// This function will be called by HGE once per frame.
// Put your game loop code here. In this example we
// just check whether ESC key has been pressed.
bool FrameFunc()
{
    dt=hge->Timer_GetDelta();
    // By returning "true" we tell HGE
    // to stop running the application.
    if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
    //�����ã�����������
    if (hge->Input_GetKeyState(HGEK_SPACE))
    {
       ent->body->WakeUp();
       ent->body->SetXForm(b2Vec2(38.8,20.0),0.0);
       ent->body->SetLinearVelocity(b2Vec2(0.0,0.0));
       score=0;
    }
    if(hge->Input_GetKeyState(HGEK_LEFT))
    {
        ent->body->ApplyForce(b2Vec2(-2.0,0.0),ent->body->GetPosition());
    }
    if(hge->Input_GetKeyState(HGEK_RIGHT))
    {

        ent->body->ApplyForce(b2Vec2(2.0,0.0),ent->body->GetPosition());
    }
    if(hge->Input_GetKeyState(HGEK_UP))
    {
        //ent->body->SetLinearVelocity(b2Vec2(0,0));
        ent->body->ApplyForce(b2Vec2(0.0,30.0),ent->body->GetPosition());
    }
    if(hge->Input_KeyDown(HGEK_S))
    {
        bumper->body->WakeUp();
        pj->SetMotorSpeed(100.0);
        pj->SetMaxMotorForce(100.0);

    }
    if(hge->Input_KeyUp(HGEK_S))
    {
        bumper->body->WakeUp();
        pj->SetMotorSpeed(-200.0);
        pj->SetMaxMotorForce(1000.0);
    }
    if(hge->Input_KeyDown(HGEK_A))
    {
        fliperL->body->WakeUp();
        fjL->SetMotorSpeed(200.0);
        fjL->SetMaxMotorTorque(1000.0);
    }
    if(hge->Input_KeyUp(HGEK_A))
    {
        fliperL->body->WakeUp();
        fjL->SetMotorSpeed(-200.0);
        fjL->SetMaxMotorTorque(1000.0);
    }
    if(hge->Input_KeyDown(HGEK_D))
    {
        fliperR->body->WakeUp();
        fjR->SetMotorSpeed(-200.0);
        fjR->SetMaxMotorTorque(1000.0);
    }
    if(hge->Input_KeyUp(HGEK_D))
    {
        fliperR->body->WakeUp();
        fjR->SetMotorSpeed(200.0);
        fjR->SetMaxMotorTorque(1000.0);
    }


    float tx,ty;
    hge->Input_GetMousePos(&tx,&ty);
    cursor->x=tx;cursor->y=ty;
    if(hge->Input_KeyDown(HGEK_LBUTTON))
    {
        //����ʱ����ȡ����Ļ�����������
        cout<<"("<<tx<<","<<ty<<")";
    }

    //�������λ������Ϊ�������������λת��Ϊ20����=1��
    ent->x=ent->body->GetPosition().x*20;
    ent->y=600-ent->body->GetPosition().y*20;
    bumper->x=bumper->body->GetPosition().x*20;
    bumper->y=600-bumper->body->GetPosition().y*20;
    fliperL->x=fliperL->body->GetPosition().x*20;
    fliperL->y=600-fliperL->body->GetPosition().y*20;
    fliperL->rotation=-fliperL->body->GetAngle();
    fliperR->x=fliperR->body->GetPosition().x*20;
    fliperR->y=600-fliperR->body->GetPosition().y*20;
    fliperR->rotation=-fliperR->body->GetAngle();

    physc->Step(timeStep,iteration);//�����������
    //��bingger����ײ�жϺʹ���
    if(pbfcontactlistener.pointscount>0)
    {
        b2Body *body1 = pbfcontactlistener.Contactbuffer[0].shape1->GetBody();
        b2Body *body2 = pbfcontactlistener.Contactbuffer[0].shape2->GetBody();
        if(body1->GetUserData()!=NULL&&body2->GetUserData()!=NULL)
        {
            if(((Entity*)(body1->GetUserData()))->id!=0||((Entity*)(body2->GetUserData()))->id!=0)
            {
                    b2Vec2 v=pbfcontactlistener.Contactbuffer[0].velocity;
                    boom(v.x*20/3-100,v.Length()*400);
                    ++score;
                    b2Vec2 normal=pbfcontactlistener.Contactbuffer[0].normal;
                    if(((Entity*)(body1->GetUserData()))->id==0)
                    {
                        normal*=-200.0;
                        body1->ApplyForce(normal,body1->GetWorldCenter());
                    }else
                    {
                        normal*=200.0;
                        body2->ApplyForce(normal,body2->GetWorldCenter());
                    }
            }
        }
    }
    pbfcontactlistener.pointscount=0;
    //�÷ִ���30����뻶��ģʽ
    if(score>=30)
    {
        b2Vec2 ballv=ent->body->GetLinearVelocity();
        par->info.nEmission=(int)ballv.Length()*400;
        par->MoveTo(ent->x,ent->y);
        par->Update(dt);
    }else
    {
        par->info.nEmission=0;
    }


    // Continue execution
    return false;
}

bool RenderFunc()
{

	hge->Gfx_BeginScene();

	// Clear screen with black color
	hge->Gfx_Clear(0);
    bkg->Render();
    ent->Render();
    bumper->Render();
    cursor->Render();
    fliperL->Render();
    fliperR->Render();
    par->Render();

    for(vector<Entity*>::iterator binger=bingers.begin();binger!=bingers.end();++binger)
    {
        (*binger)->Render();
    }

    hge->Gfx_RenderLine(pj->GetAnchor1().x*20,600-pj->GetAnchor1().y*20,pj->GetAnchor2().x*20,600-pj->GetAnchor2().y*20);
    //fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f FPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
    fnt->printf(5,30,HGETEXT_LEFT,"SCORE:");
    fnt->printf(5,50,HGETEXT_LEFT," %d",score*100);

    fnt->printf(5,360,HGETEXT_LEFT,"S:Start");
    fnt->printf(5,385,HGETEXT_LEFT,"A,D:Flipers");
    fnt->printf(5,410,HGETEXT_LEFT,"Space:");
    fnt->printf(5,435,HGETEXT_LEFT,"    Restart");
	// End rendering and update the screen/
	hge->Gfx_EndScene();

	// RenderFunc should always return false
	return false;
}

void makeRollslide()
{
     rollslide=new Entity();
     b2BodyDef rbdef;
     rbdef.position.Set(698.0/20.0,(600.0-103.0)/20.0);
     rollslide->body=physc->CreateBody(&rbdef);
     b2PolygonDef rsdef;
     rsdef.vertexCount=3;
     //��ʵ֤��һ��body�������ɶ��shape��ɣ���ȡ�������ͬ�����������Ļ���ϽǵĻ���
     rsdef.vertices[0].Set(0.0,101.0/20.0);
     rsdef.vertices[1].Set(27.0/20.0,86.0/20.0);
     rsdef.vertices[2].Set(93.0/20,101.0/20.0);
     rollslide->body->CreateShape(&rsdef);

     rsdef.vertices[0].Set(27.0/20.0,86.0/20.0);
     rsdef.vertices[1].Set(38.0/20.0,79.0/20.0);
     rsdef.vertices[2].Set(93.0/20,101.0/20.0);
     rollslide->body->CreateShape(&rsdef);

     rsdef.vertices[0].Set(38.0/20.0,79.0/20.0);
     rsdef.vertices[1].Set(58.0/20.0,61.0/20.0);
     rsdef.vertices[2].Set(93.0/20,101.0/20.0);
     rollslide->body->CreateShape(&rsdef);

     rsdef.vertices[0].Set(58.0/20.0,61.0/20.0);
     rsdef.vertices[1].Set(76.0/20.0,35.0/20.0);
     rsdef.vertices[2].Set(93.0/20,101.0/20.0);
     rollslide->body->CreateShape(&rsdef);

     rsdef.vertices[0].Set(76.0/20.0,35.0/20.0);
     rsdef.vertices[1].Set(93.0/20.0,0.0/20.0);
     rsdef.vertices[2].Set(93.0/20,101.0/20.0);
     rollslide->body->CreateShape(&rsdef);
}

//��Щײ�����ܺ���Ķ�����Ϊ������ײ��⣬��ҪΪÿ��binger���id������
Entity* makeBinger(HTEXTURE tex,float tx,float ty,int id)
{
    Entity *binger = new Entity();
    binger->id=id;
    binger->LoadSprite(tex,0,0,64,64);
    binger->x=tx;binger->y=ty;
    b2BodyDef bbdef;
    bbdef.position.Set(tx/20.0,(600-ty)/20.0);
    binger->body=physc->CreateBody(&bbdef);
    binger->body->SetUserData(binger);
    b2CircleDef bcsdef;
    bcsdef.radius=32.0/20;
    binger->body->CreateShape(&bcsdef);
    return binger;
}

bool initEngine()
{
    HTEXTURE tex;

    hge = hgeCreate(HGE_VERSION);
    hge->System_SetState(HGE_FPS,60);
    hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    hge->System_SetState(HGE_RENDERFUNC,RenderFunc);
    hge->System_SetState(HGE_TITLE, "Pin Ball Frenzy");
    hge->System_SetState(HGE_WINDOWED, true);//ȫ��
    hge->System_SetState(HGE_USESOUND, true);//��������

    border.upperBound=b2Vec2(42,32);
    border.lowerBound=b2Vec2(-2,-2);
    physc=new b2World(border,gravity,doSleep);//��ʼ����������
    physc->SetContactListener(&pbfcontactlistener);

    if(hge->System_Initiate())
    {
        //�����õ����ָ��
        cursor = new Entity();
        cursor->LoadSprite(hge->Texture_Load("cursor.png"),0,0,16,16);
        cursor->sprite->SetHotSpot(0,0);
        cursor->sprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE);

        //С���ʼ��
        ent = new Entity();
        ent->sound=hge->Effect_Load("CLANG4.WAV");
        tex=hge->Texture_Load("particles.png");
        ent->LoadSprite(tex,96,64,32,32);//��ʼ��С��
        ent->sprite->SetColor(0xff0000ff);
        ent->sprite->SetBlendMode(BLEND_COLORMUL|BLEND_ALPHABLEND|BLEND_NOZWRITE);
        b2BodyDef balldef;
        balldef.position.Set(38.8,20.0);
        ent->body=physc->CreateBody(&balldef);
        ent->body->SetUserData(ent);
        b2CircleDef ballshapedef;
        ballshapedef.radius=16.0/20.0;
        ballshapedef.density=1.0;
        ballshapedef.friction=0.5;
        ballshapedef.restitution=0.5;
        ent->body->CreateShape(&ballshapedef);
        ent->body->SetMassFromShapes();

        par=new hgeParticleSystem("trail.psi",ent->sprite);
		par->Fire();
        //���棬�����жϵ���ʱ���������
        ground = new Entity();
        b2BodyDef grounddef;
        grounddef.position.Set(15.0,0.0);
        ground->body=physc->CreateBody(&grounddef);
        b2PolygonDef groundshapedef;
        groundshapedef.SetAsBox(30.0,0.1);
        ground->body->CreateShape(&groundshapedef);

        //��ʼ��ײ�ӣ�����ײ�Ӻ͵��棬ʹ��Prasmatic joint��motor��ģ�ⵯ��
        bumper = new Entity();
        bumper->LoadSprite(hge->Texture_Load("bumper.png"),0,0,32,32);
        b2BodyDef bbdef;
        bbdef.position.Set(38.8,5.0);
        bumper->body=physc->CreateBody(&bbdef);
        b2PolygonDef bsdef;
        bsdef.SetAsBox(0.5,0.5);
        bsdef.density=10.0;
        bumper->body->CreateShape(&bsdef);
        bumper->body->SetMassFromShapes();

        b2PrismaticJointDef pjd;
        pjd.Initialize(bumper->body,ground->body,bumper->body->GetWorldCenter(),b2Vec2(0.0,1.0));
        pjd.upperTranslation=5.0;
        pjd.lowerTranslation=-5.0;
        pjd.motorSpeed=-600.0;
        pjd.maxMotorForce=200.0;
        pjd.enableMotor=true;
        pjd.enableLimit=true;
        pj=(b2PrismaticJoint*)physc->CreateJoint(&pjd);
        //cout<<pj->GetAnchor2().x<<","<<pj->GetAnchor2().y;



        //��Ϸ����ͼƬ
        bkg=new Entity();
        bkg->LoadSprite(hge->Texture_Load("background.png"),0,0,800,600);
        bkg->x=400;bkg->y=300;


        //�Ϸ��߽�
        upbound = new Entity();
        b2BodyDef ubdef;
        ubdef.position.Set(22.0,30);
        upbound->body=physc->CreateBody(&ubdef);
        b2PolygonDef usdef;
        usdef.SetAsBox(44.0,0.1);
        upbound->body->CreateShape(&usdef);

        //���߽�
        leftbound = new Entity();
        b2BodyDef lbdef;
        lbdef.position.Set(7.0,20.0);
        leftbound->body=physc->CreateBody(&lbdef);
        b2PolygonDef lsdef;
        lsdef.SetAsBox(0.1,40.0);
        leftbound->body->CreateShape(&lsdef);

        //���б��,���ڶ������״����������Ĳ��յ��Ƕ��������ԭ�㣬��������û�����ң�201296
        slideL = new Entity();
        b2BodyDef slbdef;
        slbdef.position.Set(265.0/20.0,35.0/20.0);
        slideL->body=physc->CreateBody(&slbdef);
        b2PolygonDef slsdef;
        slsdef.vertexCount=3;
        slsdef.vertices[0].Set(0.0,0.0);
        slsdef.vertices[1].Set(130.0/20.0,0.0);
        slsdef.vertices[2].Set(0.0,80.0/20);
        slideL->body->CreateShape(&slsdef);
        //�Ҳ�б��
        slideR = new Entity();
        b2BodyDef srbdef;
        srbdef.position.Set(540.0/20.0,(600.0-565.0)/20.0);
        slideR->body=physc->CreateBody(&srbdef);
        b2PolygonDef srsdef;
        srsdef.vertexCount=3;
        srsdef.vertices[0].Set(0.0,0.0);
        srsdef.vertices[1].Set(130.0/20.0,0.0);
        srsdef.vertices[2].Set(130.0/20.0,80.0/20.0);
        slideR->body->CreateShape(&srsdef);
        //�󵯰壬ʹ��b2rRevoluteJoint��motor��ʵ�ֵ���
        fliperL = new Entity();
        fliperL->LoadSprite(hge->Texture_Load("fliperL.png"),0,0,64,32);
        fliperL->sprite->SetHotSpot(9,16);
        b2BodyDef flpdef;
        flpdef.position.Set(400.0/20.0,40.0/20.0);
        fliperL->body=physc->CreateBody(&flpdef);
        b2CircleDef flpcdef;
        flpcdef.radius=9.0/20.0;
        flpcdef.density=1.0;
        fliperL->body->CreateShape(&flpcdef);
        b2PolygonDef flppdef;
        flppdef.vertexCount=3;
        flppdef.vertices[0].Set(0.0,-9.0/20.0);
        flppdef.vertices[1].Set(70.0/20.0,0.0);
        flppdef.vertices[2].Set(0.0,9.0/20.0);
        flppdef.density=1.0;
        fliperL->body->CreateShape(&flppdef);
        fliperL->body->SetMassFromShapes();

        b2RevoluteJointDef flpj;
        flpj.Initialize(slideL->body,fliperL->body,b2Vec2(400/20.0,40.0/20.0));
        flpj.enableLimit=true;
        flpj.upperAngle=0.25*b2_pi;
        flpj.lowerAngle=-1.0/6.0*b2_pi;
        flpj.enableMotor=true;
        flpj.motorSpeed=-50.0;
        flpj.maxMotorTorque=50.0;
        fjL=(b2RevoluteJoint*)physc->CreateJoint(&flpj);

        //�ҵ���
        fliperR=new Entity();
        fliperR->LoadSprite(hge->Texture_Load("fliperR.png"),0,0,64,32);
        fliperR->sprite->SetHotSpot(55,16);
        b2BodyDef frpdef;
        frpdef.position.Set(535.0/20.0,40.0/20.0);
        fliperR->body=physc->CreateBody(&frpdef);
        b2CircleDef frpcdef;
        frpcdef.radius=9.0/20;
        frpcdef.density=1.0;
        fliperR->body->CreateShape(&frpcdef);
        b2PolygonDef frppdef;
        frppdef.vertexCount=3;
        frppdef.vertices[0].Set(0.0,-9.0/20.0);
        frppdef.vertices[1].Set(0.0,9.0/20.0);
        frppdef.vertices[2].Set(-70.0/20.0,0.0);
        frppdef.density=1.0;
        fliperR->body->CreateShape(&frppdef);
        fliperR->body->SetMassFromShapes();

        b2RevoluteJointDef frpj;
        frpj.Initialize(slideR->body,fliperR->body,b2Vec2(535.0/20.0,40.0/20.0));
        frpj.enableLimit=true;
        frpj.upperAngle=1.0/6.0*b2_pi;
        frpj.lowerAngle=-0.25*b2_pi;
        frpj.enableMotor=true;
        flpj.motorSpeed=50;
        flpj.maxMotorTorque=-50.0;
        fjR=(b2RevoluteJoint*)physc->CreateJoint(&frpj);



        //�������󵲰�
        holderL=new Entity();
        b2BodyDef hlbdef;
        hlbdef.position.Set(755.0/20.0,445.0/40.0);
        holderL->body=physc->CreateBody(&hlbdef);
        b2PolygonDef hlsdef;
        hlsdef.SetAsBox(10.0/100.0,445.0/40.0);
        holderL->body->CreateShape(&hlsdef);
        //�������ҵ���
        holderR=new Entity();
        b2BodyDef hrbdef;
        hrbdef.position.Set(795.0/20.0,500.0/40.0);
        holderR->body=physc->CreateBody(&hrbdef);
        b2PolygonDef hrsdef;
        hrsdef.SetAsBox(10.0/100.0,500.0/40.0);
        holderR->body->CreateShape(&hrsdef);
        //��ʼ������
        makeRollslide();
        //��ʼ��bingers
        HTEXTURE bingTex=hge->Texture_Load("binger.png");
        bingers.push_back(makeBinger(bingTex,200,480,1));
        bingers.push_back(makeBinger(bingTex,455,105,2));
        bingers.push_back(makeBinger(bingTex,715,435,3));
        bingers.push_back(makeBinger(bingTex,320,200,4));
        bingers.push_back(makeBinger(bingTex,600,210,5));
        bingers.push_back(makeBinger(bingTex,455,290,6));


        fnt=new hgeFont("font1.fnt");

        if(!ent->sound)
        {
            MessageBox(NULL,"Resource Loading Error.", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            hge->System_Shutdown();
            hge->Release();
        }
        hge->System_Start();
        hge->Texture_Free(tex);
        hge->Texture_Free(bingTex);
        hge->Effect_Free(ent->sound);
        return true;
    }
    else
    {
        // If HGE initialization failed show error message
        MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    }

    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    initEngine();
    // Now ESC has been pressed or the user
    // has closed the window by other means.

    // Restore video mode and free
    // all allocated resources
    hge->System_Shutdown();

    // Release the HGE interface.
    // If there are no more references,
    // the HGE object will be deleted.
    hge->Release();
    delete ent;
    delete bkg;
    delete ground;
    delete slideL;
    delete slideR;
    return 0;
}
