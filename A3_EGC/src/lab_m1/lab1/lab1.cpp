#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

#define SWITCHER_MOVE_DISTANCE 0.5
#define JUMP_SPEED 2
#define JUMP_TIME 0.75

Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    clear_R = clear_G = clear_B = 0;
    switcher_target = 0;
    switcher_pos=glm::vec3(1, 1, 1);
    switcher_d_x = switcher_d_y = switcher_d_z=0;
    jump_time=fall_time=0;
    switcher_lock=0;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    // (student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    {
        Mesh* mesh = new Mesh("screen_quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "screen_quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // (student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(clear_R, clear_G, clear_B, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // (student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    Mesh *target_mesh;
    switch(switcher_target)
    {
        case 0: target_mesh=meshes["sphere"]; break;
        case 1: target_mesh=meshes["teapot"]; break;
        case 2: target_mesh=meshes["quad"]; break;
        case 3: target_mesh=meshes["screen_quad"]; break;
    }
    #define SWICHER_OPTIONS 4

    switcher_pos.x+=deltaTimeSeconds*switcher_d_x;
    
    switcher_pos.y+=deltaTimeSeconds*switcher_d_y;
    switcher_pos.z+=deltaTimeSeconds*switcher_d_z;

    RenderMesh(target_mesh, switcher_pos);
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

    if(jump_time !=0)
    {
        float jump_time_d=min(jump_time, deltaTime)/JUMP_TIME;
        float jump_dist=cos(jump_time) - cos(jump_time-jump_time_d);
        GetSceneCamera()->MoveUp(-JUMP_SPEED*jump_dist);
        jump_time-=jump_time_d;
        
        if(jump_time<=0)
        {
            jump_time=0;
            fall_time=1;
        }
    }
    else if(fall_time !=0)
    {
        float fall_time_d=min(fall_time, deltaTime)/JUMP_TIME;
        float fall_dist=cos(fall_time) - cos(fall_time-fall_time_d);
        GetSceneCamera()->MoveUp(JUMP_SPEED*fall_dist);
        fall_time-=fall_time_d;

    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // (student): Change the values of the color components.
       clear_R = clear_G = ~ clear_G;
    }

    // (student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

    if (key==GLFW_KEY_R) {
        switcher_target++;
        switcher_target%=SWICHER_OPTIONS;
    }
    if( !switcher_lock)
    {    
        if (key==GLFW_KEY_Q)
            switcher_d_y += SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_W)
            switcher_d_x += SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_A)
            switcher_d_z += SWITCHER_MOVE_DISTANCE;

        if (key==GLFW_KEY_E)
            switcher_d_y -= SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_S)
            switcher_d_x -= SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_D)
            switcher_d_z -= SWITCHER_MOVE_DISTANCE;
    }

    if (key==32 && fall_time==0 && jump_time==0)
    {
        jump_time=1;
    }
 
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if(!switcher_lock)
    {
        if (key==GLFW_KEY_Q)
            switcher_d_y -= SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_W)
            switcher_d_x -= SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_A)
            switcher_d_z -= SWITCHER_MOVE_DISTANCE;

        if (key==GLFW_KEY_E)
            switcher_d_y += SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_S)
            switcher_d_x += SWITCHER_MOVE_DISTANCE;
        if (key==GLFW_KEY_D)
            switcher_d_z += SWITCHER_MOVE_DISTANCE;
    }

}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if(button==2)
        switcher_lock=1;
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if(button==2)
        switcher_lock=0;
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
