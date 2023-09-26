#include "lab_m1/tema1/Duck.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

#include "transform2D.h"
#include "object2D.h"

#include "utils/glm_utils.h"

using namespace std;
using namespace m1;

std::unordered_map<std::string, Mesh*> Duck::meshes;
std::unordered_map<std::string, glm::mat3> Duck::translates;
std::list<std::string> Duck::render_elements;
float Duck::coalisionX[2], Duck::coalisionY[2];
float Duck::base_fly_speed=40;


#define DUCK_LENGTH 18.5f

float mod(float a)
{
    if(a>0)
        return a;
    return -a;
}

#define rand_in_range(start, fin) ((int)(start)) + ( std::rand() % ((int) ((int)(fin)) - ((int)(start)) + 1 ) )


Duck::Duck()
{
    scaleX=scaleY=0.75;
    rotation_angle=RADIANS(15)+ RADIANS(90)*(rand()%2) + RADIANS(rand()%60);
	y=Duck::coalisionY[0];
	if(Duck::coalisionX[0]==0)
		x=0;
	else
	{
		int max=-Duck::coalisionX[0]*10+ Duck::coalisionX[1]*10 - 4.0f*DUCK_LENGTH*scaleX;
		x=rand_in_range(0, max);
		// change center to 0
		x-=mod(Duck::coalisionX[0]*10);
		// add margin
		x+=DUCK_LENGTH*scaleX*2.f;
		// scale back
		x/=10;
	}
	fly_speed=Duck::base_fly_speed;

    state=DuckStates::free_flying;

	wingFlapSpeed=3;
	wingFlapState=false;
	wingFlapAngle=1;

    onScreenColision=true;
    recalcBaseMatrix();

    escapeTimer=timer::Timer(DUCK_ESCAPE_AFTER);
}

Duck::~Duck()
{

}

void Duck::tick(float deltaTime)
{
	if(state==DuckStates::free_flying)
		escapeTimer.progress(deltaTime);
	if(state==DuckStates::crashed)
	    crashTimer.progress(deltaTime);

    if(escapeTimer.isDone() && state==DuckStates::free_flying)
    {
        escape();
    }

    if(state==DuckStates::free_flying ||
        state==DuckStates::escaping)
	   tickWing(deltaTime);

    if(state==DuckStates::free_flying)
        tickFreeFly(deltaTime);

    if(state==DuckStates::shot_down)
    {
        x+=deltaTime*forcedMove.x;
        y+=deltaTime*forcedMove.y;

		if(Duck::coalisionY[0]<y-0.5*DUCK_LENGTH*mod(sin(rotation_angle))*scaleX)
	        forcedMove.y-=100*deltaTime;
		else
		{
			if(!crashed)
			{
				crashed=true;
				state=DuckStates::crashed;
				crashTimer=timer::Timer(DUCK_BODY_LIFE);
			}

			forcedMove.y+=10*deltaTime;
			forcedMove.y=max(0.f, forcedMove.y);
			forcedMove.x-=10*forcedMove.x*deltaTime;
		}
    }

    if(state==DuckStates::escaping)
    {
        x+=deltaTime*forcedMove.x;
        y+=deltaTime*forcedMove.y;

        if(y>3*Duck::coalisionY[1])
            state=DuckStates::escaped;


        forcedMove.y+=200*deltaTime;
    }

	recalcBaseMatrix();
}

void Duck::tickFreeFly(float deltaTime)
{
    glm::vec3 move=transform2D::Rotate(rotation_angle)
    * glm::vec3(deltaTime*fly_speed, 0, 0);
    x+=move[0];
    y+=move[1];

	// std::cout<<"Duck["<<x<<"\t"<<y<<"]\n";
	// std::cout<<Duck::coalisionX[0]<<"\t"<<Duck::coalisionX[1]<<"\t"<<Duck::coalisionY[0]<<"\t"<<Duck::coalisionY[1]<<"\n";

    if(onScreenColision)
    {
        float x_length=DUCK_LENGTH*mod(cos(rotation_angle))*scaleX;
        float y_length=DUCK_LENGTH*(sin(rotation_angle))*scaleX;

    	if(Duck::coalisionX[0]>x-x_length)
    	{
    		rotation_angle=-(rotation_angle-3.1415f);
    		x-=move[0];
    	}
    	if(Duck::coalisionX[1]<x+x_length)
    	{
    		rotation_angle=-(rotation_angle-3.1415f);
    		x-=move[0];
    	}
    	if(Duck::coalisionY[0]>y+y_length)
    	{
    		rotation_angle=-rotation_angle;
    		y-=move[1];
    	}
    	if(Duck::coalisionY[1]<y+mod(y_length))
    	{
    		rotation_angle=-rotation_angle;
    		y-=move[1];
    	}
    }

    // normalize rotation_angle
	while(rotation_angle>2*3.1415f)
        rotation_angle-=2*3.1415f;
	while(rotation_angle<0)
        rotation_angle+=2*3.1415f;
}

void Duck::tickWing(float deltaTime)
{
    if(wingFlapState)
	{
		wingFlapAngle+=deltaTime*wingFlapSpeed;
		if(wingFlapAngle>1)
		{
			wingFlapState=false;
			wingFlapAngle=1-(wingFlapAngle-1);
		}
	}
	else
	{
		wingFlapAngle-=deltaTime*wingFlapSpeed;
		if(wingFlapAngle<-1)
		{
			wingFlapState=true;
			wingFlapAngle=-1-(wingFlapAngle+1);
		}
	}
    setFlapAngle(wingFlapAngle);
}

void Duck::recalcBaseMatrix()
{
    baseMatrix=glm::mat3(1);
    baseMatrix *= transform2D::Translate(x,y);
    baseMatrix *= transform2D::Scale(scaleX, scaleY);
    baseMatrix *= transform2D::Rotate(rotation_angle);

    // fix fly upsidedown
    if(RADIANS(90)<rotation_angle && rotation_angle<RADIANS(270))
        baseMatrix *= transform2D::Scale(1, -1);
}

bool Duck::clickedOn(float ClickX, float ClickY)
{
    glm::mat3 tempMatrix=glm::mat3(1)
                * transform2D::Translate(-4, -3)
                * transform2D::Rotate(-rotation_angle)
                * transform2D::Translate(-x, -y);

    glm::vec3 temp=tempMatrix* glm::vec3(ClickX, ClickY, 1);
    float dX=mod(temp.x)/scaleX;
    float dY=1.75*mod(temp.y)/scaleY;
    // cout<<" \t"<<dX<<" "<<dY<<" "<<(dX*dX)+(dY*dY)<<" \t";

    return (dX*dX)+(dY*dY)<DUCK_LENGTH*DUCK_LENGTH;
}

void Duck::escape()
{
    state=DuckStates::escaping;
    forcedMove=transform2D::Rotate(rotation_angle)*
                glm::vec3(fly_speed, 0, 1);

    if(RADIANS(180)<rotation_angle && rotation_angle<RADIANS(270))
        rotation_angle=((RADIANS(180)-(rotation_angle-RADIANS(180))));

    if(RADIANS(270)<rotation_angle)
        rotation_angle=((RADIANS(90)-(rotation_angle-RADIANS(270))));

    fly_speed=0;
}

void Duck::shot()
{
    state=DuckStates::shot_down;
    forcedMove=transform2D::Rotate(rotation_angle)*
                glm::vec3(fly_speed, 0, 1);
    fly_speed=0;
    setFlapAngle(1);
}

void Duck::setFlapAngle(float state)
{
	Duck::translates.erase("wing_forward");
	Duck::translates.insert({"wing_forward",
		transform2D::Translate(-7,0)*transform2D::Scale(20, 15*sin(3.14*state/2))});
}

void Duck::InitMesh()
{
    // Duck.meshes=std::unordered_map
    // meshes=std::unordered_map<std::string, Mesh *>();

    Duck::meshes.insert({
		{"body", object2D::CreateElipse("body", 			 glm::vec3(0,0,0), 10  , 3  , NormalizedRGB(219, 182, 101), true)},
        {"neck", object2D::CreateElipse("neck", 			 glm::vec3(0,0,0),  5  , 2  , NormalizedRGB(150,  75,   0), true)},
		{"head", object2D::CreateElipse("head", 			 glm::vec3(0,0,0),  3.5, 3.5, NormalizedRGB( 52, 235, 143), true)},
		{"eye",  object2D::CreateElipse("eye",				 glm::vec3(0,0,0),  0.7, 0.7, NormalizedRGB(  0,   0,   0), true)},
		{"beak", object2D::CreateTriangleEquilateral("beak", glm::vec3(0,0,0),  3,        NormalizedRGB(255, 223,   0), true)},
		{"leg", object2D::CreateSquare("leg", glm::vec3(0,0,0),  1,        NormalizedRGB(255, 165,   0), true)},
        {"wing_forward", object2D::CreateDuckWing("wing_forward", glm::vec3(0,0,0),  	  NormalizedRGB(228, 218, 206), true)},
    });
	Duck::translates.insert({
		{"body", glm::mat3(1)},
		{"wing_forward", transform2D::Translate(-7,0)*transform2D::Scale(20, 15)},
		{"leg", transform2D::Translate(-10  ,-2.7)*transform2D::Rotate(0.3)*transform2D::Scale(3, 0.5f)},
        {"neck", transform2D::Translate(10  ,1.2)*transform2D::Rotate(0.6)},
		{"head", transform2D::Translate(13.2,5.5)},
		{"eye", transform2D::Translate(12.5,7.0)},
		{"beak", transform2D::Translate(15  ,4.0)* transform2D::Scale(1.5, .75f)* transform2D::Rotate(0.5)},
    });
	// setFlapAngle(1);
	// glm::mat3 a=glm::mat3(1);
	// string b="body";
	// Duck::translates.insert({b, a});

	Duck::render_elements.push_back("wing_forward");
	Duck::render_elements.push_back("beak");
	Duck::render_elements.push_back("eye");
	Duck::render_elements.push_back("leg");
	Duck::render_elements.push_back("head");
	Duck::render_elements.push_back("neck");
	Duck::render_elements.push_back("body");
}

void Duck::draw(gfxc::SimpleScene scene)
{

    // scene.RenderMesh2D(meshes["body"], scene.shaders["VertexColor"], glm::mat3(1));
    // meshes["body"]
}
