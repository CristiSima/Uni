#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "transform2D.h"
#include "object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
#define MOVE_DISTANCE 1
#define GRASS_HEIGHT 5

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

	float y=GetSceneCamera()->GetFieldOfViewY();
	float x=GetSceneCamera()->GetFieldOfViewX();
    OnWindowResize(x, y);
	camera->Update();


	glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

	// window->props.name="ASD";
    // window->HidePointer();


    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;

    Mesh* grass = object2D::CreateSquare("grass", corner, length, NormalizedRGB(82, 252, 3), true);
    AddMeshToList(grass);

    grass = object2D::CreateTarget("target", glm::vec3(0,0,0), 2.f, 1.5f, NormalizedRGB(0, 0, 0), true);
    AddMeshToList(grass);

	grass = object2D::CreateBuletIcon("bullet", glm::vec3(0,0,0), NormalizedRGB(0, 0, 0), true);
    AddMeshToList(grass);

	grass = object2D::CreateSquare("progress_esge", glm::vec3(0,0,0), 1, NormalizedRGB(0, 0, 0), true);
    AddMeshToList(grass);

	grass = object2D::CreateSquare("progress_outline", glm::vec3(0,0,0), 1, NormalizedRGB(255, 255, 255), true);
    AddMeshToList(grass);

	grass = object2D::CreateSquare("progress_green", glm::vec3(0,0,0), 1, NormalizedRGB(10, 255, 10), true);
    AddMeshToList(grass);

	grass = object2D::CreateSquare("progress_orange", glm::vec3(0,0,0), 1, NormalizedRGB(255, 165, 0), true);
	AddMeshToList(grass);

	grass = object2D::CreateSquare("progress_red", glm::vec3(0,0,0), 1, NormalizedRGB(255, 0, 0), true);
	AddMeshToList(grass);

	textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
	textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 80);


    mouseX=mouseY=0;
	ammo_left=3;
	max_ammo=3;
	ducks_shot=0;


    Duck::InitMesh();
    rata=Duck();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2, 0.2, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
	rata.tick(deltaTimeSeconds);
    // if(rata.state==DuckStates::crashed)
    if(rata.state==DuckStates::escaped)
    {
        // rata=Duck();
    }


    RenderMesh2D(meshes["target"], shaders["VertexColor"],
        transform2D::Translate(mouseX, mouseY)
    );

	#define BULLED_LENGTH 4.f
	if(ammo_left)
	for(int i=0;i<ammo_left;i++)
		RenderMesh2D(meshes["bullet"], shaders["VertexColor"],
			transform2D::Translate(52+(i)*BULLED_LENGTH-max_ammo*BULLED_LENGTH, 24)*
			transform2D::Scale(0.4f,0.4f)*
			transform2D::Rotate(RADIANS(60))
		);




	string progress;
	if (rata.escapeTimer.persantage_left()>0.66f)
		progress="progress_green";
	else if (rata.escapeTimer.persantage_left()>0.33f)
		progress="progress_orange";
	else
		progress="progress_red";

	RenderMesh2D(meshes[progress], shaders["VertexColor"],
		transform2D::Translate(52-max_ammo*BULLED_LENGTH-1.f, 21.5f)*
		transform2D::Scale(
			((float)max_ammo*BULLED_LENGTH-1.f)*rata.escapeTimer.persantage_left(),
			1.f
		)
	);


	RenderMesh2D(meshes["progress_outline"], shaders["VertexColor"],
		transform2D::Translate(52-max_ammo*BULLED_LENGTH-1.2f, 21.3f)*
		transform2D::Scale((float)max_ammo*BULLED_LENGTH-0.6f, 1.4f)
	);

	RenderMesh2D(meshes["progress_esge"], shaders["VertexColor"],
		transform2D::Translate(52-max_ammo*BULLED_LENGTH-1.5f, 21)*
		transform2D::Scale((float)max_ammo*BULLED_LENGTH, 2)
	);


	textRenderer->RenderText(
		"Ducks shot: "+std::to_string(ducks_shot),
		15.0f, 15.f, .25f, NormalizedRGB(0,0,0)
	);


	if(rata.state==DuckStates::escaped)
	{
		// glm::ivec2 resolution = window->GetResolution();
		// cout<<"resolution: "<<resolution<<"\n";
		float scale=1.f;
		float height=36.f    *scale;
		float offset1=5.f  *scale;
		float x=1280/2 - 260.f*scale;
		float y= 720/2 - 30.f;
		textRenderer->RenderText("  YOU LOST ", x-offset1, y-height, scale, NormalizedRGB(0,0,0));
		textRenderer->RenderText("LOL YOU BAD", x,         y+height, scale, NormalizedRGB(0,0,0));
	}


	Duck_draw(rata);
	RenderMesh2D(meshes["grass"], shaders["VertexColor"], grassMatrix);


	if(rata.state==DuckStates::crashed && rata.crashTimer.isDone())
	{
		rata=Duck();
	}
}


void Tema1::FrameEnd()
{
}




/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    float ScreenX=(float)GetSceneCamera()->GetFieldOfViewX()*mouseX/window->GetResolution()[0]
            -GetSceneCamera()->GetFieldOfViewX()/2;
    float ScreenY=-((float)GetSceneCamera()->GetFieldOfViewY()*mouseY/window->GetResolution()[1]
             -GetSceneCamera()->GetFieldOfViewY()/2);

    this->mouseX=ScreenX;
    this->mouseY=ScreenY;


}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    float ScreenX=(float)GetSceneCamera()->GetFieldOfViewX()*mouseX/window->GetResolution()[0]
            -GetSceneCamera()->GetFieldOfViewX()/2;
    float ScreenY=-((float)GetSceneCamera()->GetFieldOfViewY()*mouseY/window->GetResolution()[1]
             -GetSceneCamera()->GetFieldOfViewY()/2);
    // cout<<ScreenX<<" "<<ScreenY<<" "<<rata.clickedOn(ScreenX, ScreenY)<<"\n";

    if(button&1 && ammo_left>0 && rata.state==DuckStates::free_flying)
    {
        // tried to shot
        // has ammo
        // can shot

        // use ammo
        ammo_left--;

        // check if targeted the duck
        if (rata.clickedOn(ScreenX, ScreenY))
        {
            rata.shot();
            ducks_shot++;
			if(ducks_shot%5==0)
				max_ammo++;
			ammo_left=max_ammo;
			Duck::base_fly_speed+=5;
        }
    }

	if(button&1 && rata.state==DuckStates::escaped)
	{
		rata=Duck();
		ammo_left=max_ammo=3;
		ducks_shot=0;
	}
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
    glViewport(0, 0, width, height);
	// cout<<"size: "<<width<<" "<<height<<"\n";

	float y=GetSceneCamera()->GetFieldOfViewY();
	float x=GetSceneCamera()->GetFieldOfViewX();

    // cout<<"size: "<<x<<" "<<y<<"\n";
	Duck::coalisionX[0]=-x/2;
	Duck::coalisionX[1]= x/2;
	Duck::coalisionY[0]=-y/2+GRASS_HEIGHT;
	Duck::coalisionY[1]= y/2;

	// std::cout<<Duck::coalisionX[0]<<"\t"<<Duck::coalisionX[1]<<"\t"<<Duck::coalisionY[0]<<"\t"<<Duck::coalisionY[1]<<"\n";

	grassMatrix=transform2D::Translate(-x/2, -y/2)*transform2D::Scale(x, GRASS_HEIGHT);
}
