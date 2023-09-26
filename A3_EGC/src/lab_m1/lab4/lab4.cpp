#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

#define TRANSLATE_DISTANCE 3
#define SCALE_CONST 1
#define RPM 1
#define ORBIT1_DISTANCE 3
#define ORBIT1_SPEED 1
#define ORBIT1_SCALE 0.5
#define ORBIT1_SELF_SPEED 2.4
#define ORBIT2_DISTANCE 2.2
#define ORBIT2_SCALE 0.25
#define ORBIT2_SPEED 2
#define ORBIT2_SELF_SPEED -5.4

Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    inputW = inputA = inputS = inputD=0;
    camera_control=false;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    modelMatrix *= transform3D::Translate(ORBIT1_DISTANCE*sin(3.14*ORBIT1_SPEED*orbit1), 0, ORBIT1_DISTANCE*cos(3.14*ORBIT1_SPEED*orbit1));
    modelMatrix *= transform3D::RotateOY(ORBIT1_SELF_SPEED*orbit1);
    modelMatrix *= transform3D::Scale(ORBIT1_SCALE, ORBIT1_SCALE, ORBIT1_SCALE);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    modelMatrix *= transform3D::Scale(ORBIT2_SCALE/ORBIT1_SCALE, ORBIT2_SCALE/ORBIT1_SCALE, ORBIT2_SCALE/ORBIT1_SCALE);
    modelMatrix *= transform3D::RotateOY(-ORBIT1_SELF_SPEED*orbit2);
    modelMatrix *= transform3D::Translate(ORBIT2_DISTANCE*sin(3.14*ORBIT2_SPEED*orbit2), 0, ORBIT2_DISTANCE*cos(3.14*ORBIT2_SPEED*orbit2));
    modelMatrix *= transform3D::RotateOY(ORBIT2_SELF_SPEED*orbit2);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    DrawCoordinateSystem();
    RenderScene();

    orbit1+=deltaTimeSeconds;
    orbit2+=deltaTimeSeconds;
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if(inputD) translateX+=TRANSLATE_DISTANCE*deltaTime;
    if(inputA) translateX-=TRANSLATE_DISTANCE*deltaTime;
    if(inputR) translateY+=TRANSLATE_DISTANCE*deltaTime;
    if(inputF) translateY-=TRANSLATE_DISTANCE*deltaTime;
    if(inputW) translateZ-=TRANSLATE_DISTANCE*deltaTime;
    if(inputS) translateZ+=TRANSLATE_DISTANCE*deltaTime;


    if(input_num[1]) scaleX=scaleY=scaleZ=scaleZ+SCALE_CONST*deltaTime;
    if(input_num[2]) scaleX=scaleY=scaleZ=max(0.1f, scaleZ-SCALE_CONST*deltaTime);

    if(input_num[3]) angularStepOX+=RPM*3.14f*deltaTime;
    if(input_num[4]) angularStepOX-=RPM*3.14f*deltaTime;
    if(input_num[5]) angularStepOY+=RPM*3.14f*deltaTime;
    if(input_num[6]) angularStepOY-=RPM*3.14f*deltaTime;
    if(input_num[7]) angularStepOZ+=RPM*3.14f*deltaTime;
    if(input_num[8]) angularStepOZ-=RPM*3.14f*deltaTime;
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if(!camera_control)
    {
        if( key == GLFW_KEY_W)  inputW=true;
        if( key == GLFW_KEY_A)  inputA=true;
        if( key == GLFW_KEY_S)  inputS=true;
        if( key == GLFW_KEY_D)  inputD=true;
        if( key == GLFW_KEY_R)  inputR=true;
        if( key == GLFW_KEY_F)  inputF=true;
    }
    if( GLFW_KEY_0 <=key && key <=GLFW_KEY_9)
        input_num[key-GLFW_KEY_0]=true;

    // TODO(student): Add viewport movement and scaling logic
    if(key==GLFW_KEY_L) miniViewportArea.x+=10;
    if(key==GLFW_KEY_J) miniViewportArea.x-=10;
    if(key==GLFW_KEY_I) miniViewportArea.y+=10;
    if(key==GLFW_KEY_K) miniViewportArea.y-=10;

    if(key==GLFW_KEY_U || key==GLFW_KEY_O)
    {
        int direction=(key==GLFW_KEY_U)?-1:1;
        miniViewportArea.height+=10*direction;
        miniViewportArea.width+=10*direction;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if( key == GLFW_KEY_W)  inputW=false;
    if( key == GLFW_KEY_A)  inputA=false;
    if( key == GLFW_KEY_S)  inputS=false;
    if( key == GLFW_KEY_D)  inputD=false;
    if( key == GLFW_KEY_R)  inputR=false;
    if( key == GLFW_KEY_F)  inputF=false;
    if( GLFW_KEY_0 <=key && key <=GLFW_KEY_9)
        input_num[key-GLFW_KEY_0]=false;
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if(button==2)
    {
        camera_control=true;
        inputW = inputA = inputS = inputD=inputR=inputF=0;
    }
    for(int i=0;i<10;i++) input_num[i]=0;
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if(button==2)
        camera_control=false;
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
