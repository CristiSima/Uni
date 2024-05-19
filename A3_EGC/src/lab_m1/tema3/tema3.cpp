#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

 Mesh* CreateCone(
     const std::string &name,
     glm::vec3 color=glm::vec3(0),
     bool fill=true)
 {
	 int STEPS=100;
     // std::cout<<color<<"\n";
     std::vector<VertexFormat> vertices = { VertexFormat(glm::vec3(0,1,0), color) };
	 for(float radius=1;radius>0;radius-=0.05)
	     for(int i=1;i<=STEPS;i++)
	         vertices.push_back(VertexFormat(glm::vec3(radius*sin((float)2*3.1415f*i/STEPS), 1-radius, radius*cos((float)2*3.1415f*i/STEPS)), color));

     std::vector<unsigned int> indices = { };

	 for(int mult=0;mult<20;mult++)
	 {
		 for(int i=1;i<=STEPS;i++)
		 {
			 indices.push_back(i  + mult   *STEPS);
			 indices.push_back(i+1+ mult   *STEPS);
			 indices.push_back(i  +(mult+1)*STEPS);
			 // std::cout << i  + mult   *STEPS<<" "<<i+1+ mult   *STEPS<<" "<<i  +(mult+1)*STEPS << " "<<i+1+(mult+1)*STEPS<< '\n';

			 indices.push_back(i  +(mult+1)*STEPS);
			 indices.push_back(i+1+(mult+1)*STEPS);
			 indices.push_back(i+1+ mult   *STEPS);

		 }

		 indices.push_back(STEPS+ mult   *STEPS);
		 indices.push_back(1    + mult   *STEPS);
		 indices.push_back(STEPS+(mult+1)*STEPS);

		 indices.push_back(STEPS    +(mult+1)*STEPS);
		 indices.push_back(1  +(mult+1)*STEPS);
		 indices.push_back(1    + mult   *STEPS);

	 }

     Mesh* square = new Mesh(name);

     if (!fill) {
         std::cout<<"AI UITAT SA IMPLEMENTEZI AICI \n";
         exit(0);
     }

     square->InitFromData(vertices, indices);
     return square;
 }

Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}
#undef UPPER_BOUND


#define OBJECT_INCLINE 30
#define CAMERA_INCLINE 30
#define DESENT_SPEED 3
#define LOWER_BOUND 20
#define UPPER_BOUND -20
#define LATERAL_EDGE 15

#define OPSTACLE_POPULATION 80


void Tema3::Init()
{
	projectionMatrix=glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	camera = new implemented::Camera_T3();
	float distance_from_player=14;
	camera->Set(glm::vec3(14*sin(RADIANS(CAMERA_INCLINE)), 14*cos(RADIANS(CAMERA_INCLINE)), 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures");
    const string sourcePrimitivesDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "primitives");

    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Snow.png").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "log.jpg").c_str(), GL_REPEAT);
        mapTextures["log"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Leaf.png").c_str(), GL_REPEAT);
        mapTextures["leaf"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Present.png").c_str(), GL_REPEAT);
        mapTextures["present"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "redstone_lamp.png").c_str(), GL_REPEAT);
        mapTextures["redstone_lamp"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ski.png").c_str(), GL_REPEAT);
        mapTextures["ski"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props", "concrete.png").c_str(), GL_REPEAT);
        mapTextures["concrete"] = texture;
    }

    {
        mapTextures["random"] = CreateRandomTexture(25, 25);
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
		Mesh* mesh = new Mesh("cone");
		mesh->LoadMesh(sourcePrimitivesDir, "cone.obj");
		meshes[mesh->GetMeshID()] = mesh;
    }

    {
		Mesh* mesh=CreateCone("cone2");
		// Mesh* mesh = new Mesh("test");
		// mesh->LoadMesh(sourcePrimitivesDir, "test.obj");
		meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple quad
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(0.5f,   0.5f, 0.0f),    // top right
            glm::vec3(0.5f,  -0.5f, 0.0f),    // bottom right
            glm::vec3(-0.5f, -0.5f, 0.0f),    // bottom left
            glm::vec3(-0.5f,  0.5f, 0.0f),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            // (student): Complete texture coordinates for the square
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),

        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

	{
		glm::ivec2 resolution = window->GetResolution();
		textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
		textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 80);

	}


	reset_game();
}

void Tema3::reset_game()
{
	trees_pos={{1,0,-5}, {10, 0, 0}};
	gifts_pos={{2,0,0}, {LOWER_BOUND, 0, 0}};
	rocks_pos={{-3, 0, 0}, {UPPER_BOUND*2, 0, 0}, {0, 0, LATERAL_EDGE/2}};
	poles_pos={{0, 0, 5}, {UPPER_BOUND, 0, 0}};

	ground_texture_offset=glm::vec2(0);


	mooving=true;
	look_angle=0;
	score=0;
}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::draw()
{
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
		RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, mapTextures["snow"], true);
	}

	// drawPlayer(glm::vec3(0), look_angle);
	drawPlayer(glm::vec3(0), look_angle);

	// drawTree(glm::vec3(1, 0, 2));
	//
	// drawGift(glm::vec3(2, 0, 4));
	//
	// drawRock(glm::vec3(3, 0, 6));
	//
	// drawPole(glm::vec3(4, 0, 8));


	for (auto tree_pos: trees_pos)
		drawTree(tree_pos);

	for (auto gift_pos: gifts_pos)
		drawGift(gift_pos);

	for (auto rock_pos: rocks_pos)
		drawRock(rock_pos);

	for (auto Pole_pos: poles_pos)
		drawPole(Pole_pos);


	if(!mooving)
	{
		float scale=1.f;
		float height=36.f    *scale;
		float offset1=5.f  *scale;
		float x=1280/2 - 260.f*scale;
		float y= 720/2 - 30.f;
		textRenderer->RenderText("  YOU LOST ", x-offset1, y-height, scale, NormalizedRGB(0,0,0));
		textRenderer->RenderText("LOL YOU BAD", x,         y+height, scale, NormalizedRGB(0,0,0));
	}

	textRenderer->RenderText(
		"Presents rescued: "+std::to_string(score),
		15.0f, 15.f, .25f, NormalizedRGB(0,0,0)
	);
}

void Tema3::drawTree(glm::vec3 pos)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(.5f, 2, .5f));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["log"]);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 3, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2, 1));
	RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["leaf"]);
	// RenderSimpleMesh(meshes["cone2"], shaders["LabShader"], modelMatrix, mapTextures["log"]);
}

void Tema3::drawRock(glm::vec3 pos)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2,1,2));
	RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["concrete"]);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.4, -0.5));
	// modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5,1,0.5));
	RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["concrete"]);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
	RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["concrete"]);
}

void Tema3::drawGift(glm::vec3 pos)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
	modelMatrix = glm::translate(modelMatrix, pos);
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["present"]);
}

void Tema3::drawPlayer(glm::vec3 pos, float look_angle)
{
	float PLAYER_SKI_DISTANCE = 0.5f;
	float PLAYER_SKI_LENGTH = 2;


	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, look_angle, glm::vec3(0, 1, 0));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, PLAYER_SKI_DISTANCE/2));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PLAYER_SKI_LENGTH, 0.1f, 0.1f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["ski"]);
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, look_angle, glm::vec3(0, 1, 0));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -PLAYER_SKI_DISTANCE/2));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(PLAYER_SKI_LENGTH, 0.1f, 0.1f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["ski"]);


	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, look_angle, glm::vec3(0, 1, 0));
	// modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 1));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.1, 0));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["crate"]);
}

#define POLE_HEIGHT 5
#define POLE_ARM_LENGTH 2
inline glm::vec3 pole_pos_left(glm::vec3 pos)
{
	return pos + glm::vec3(
		glm::rotate(glm::mat4(1), RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1))*
		glm::vec4(0, POLE_HEIGHT, POLE_ARM_LENGTH, 1)
	);
}
inline glm::vec3 pole_pos_right(glm::vec3 pos)
{
	return pos + glm::vec3(
		glm::rotate(glm::mat4(1), RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1))*
		glm::vec4(0, POLE_HEIGHT, -POLE_ARM_LENGTH, 1)
	);
}

void Tema3::drawPole(glm::vec3 pos)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(.5f, POLE_HEIGHT+1, .5f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["log"]);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, POLE_HEIGHT, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(.2f, 0.2f, POLE_ARM_LENGTH*2));
	// modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["concrete"]);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, POLE_HEIGHT, POLE_ARM_LENGTH));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, .4f));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["redstone_lamp"]);
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, POLE_HEIGHT, -POLE_ARM_LENGTH));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, .4f));
	RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["redstone_lamp"]);
}

bool is_outside_upper_bound(glm::vec3 pos)
{
	return pos.x<UPPER_BOUND;
}

bool is_gift_in_range(glm::vec3 pos)
{
	return glm::length(pos)<1;
}

void Tema3::Update(float deltaTimeSeconds)
{
	// TODO: move obstacles
	glm::mat4 pos_transform=glm::rotate(glm::mat4(1), look_angle, glm::vec3(0, 1, 0));
	glm::vec3 pos_change=glm::vec3(pos_transform*glm::vec4(-DESENT_SPEED*deltaTimeSeconds, 0, 0, 1));
	if(!mooving)
		pos_change=glm::vec3(0);

	#define MAGIC_TEXTURE_OFFSET_SCALE 50

	ground_texture_offset-=glm::vec2(pos_change.x/MAGIC_TEXTURE_OFFSET_SCALE, pos_change.z/MAGIC_TEXTURE_OFFSET_SCALE);
	// std::cout << ground_texture_offset << '\n';

	for(auto &pos: trees_pos)
		pos+=pos_change;
	for(auto &pos: gifts_pos)
		pos+=pos_change;
	for(auto &pos: rocks_pos)
		pos+=pos_change;
	for(auto &pos: poles_pos)
		pos+=pos_change;

	// TODO: check outside & remove
	trees_pos.remove_if(is_outside_upper_bound);
	gifts_pos.remove_if(is_outside_upper_bound);
	rocks_pos.remove_if(is_outside_upper_bound);
	poles_pos.remove_if(is_outside_upper_bound);

	// TODO: add new entities
	int obstacles=trees_pos.size()+gifts_pos.size()+rocks_pos.size()+poles_pos.size();
	int new_obstacles=OPSTACLE_POPULATION-obstacles;
	new_obstacles=rand()%(new_obstacles+1);
	for(int i=new_obstacles/2+2;i;i--)
		new_obstacles=rand()%(new_obstacles+1);

	// std::cout << new_obstacles<<"/"<< OPSTACLE_POPULATION-obstacles<< '\n';

	for(int i=0;i<new_obstacles;i++)
		switch (rand()%4) {
			case 0:
				trees_pos.push_back({LOWER_BOUND, 0, (rand()%(1+2*LATERAL_EDGE))-LATERAL_EDGE});
			break;
			case 1:
				gifts_pos.push_back({LOWER_BOUND, 0, (rand()%(1+2*LATERAL_EDGE))-LATERAL_EDGE});
			break;
			case 2:
				rocks_pos.push_back({LOWER_BOUND, 0, (rand()%(1+2*LATERAL_EDGE))-LATERAL_EDGE});
			break;
			case 3:
				poles_pos.push_back({LOWER_BOUND, 0, (rand()%(1+2*LATERAL_EDGE))-LATERAL_EDGE});
			break;
		}

	// TODO: check colision
	for(auto pos: trees_pos) if(glm::length(pos)<1)
		mooving=false;
	for(auto pos: rocks_pos) if(glm::length(pos)<1.65)
		mooving=false;
	for(auto pos: poles_pos) if(glm::length(pos)<0.5)
		mooving=false;

	// TODO: check gifts
	for(auto pos: gifts_pos) if(is_gift_in_range(pos))
	{
		score++;
		std::cout << "PICKED GIFT\t"<< score << '\n';
	}
	gifts_pos.remove_if(is_gift_in_range);

	draw();
}


void Tema3::FrameEnd()
{
    // DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture, bool offset_texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Bind projection matrix
    // glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	// TODO: pass light sources
	glUniform1f(glGetUniformLocation(shader->program, "spot_light_limit"), cos(RADIANS(60)));
	glUniform1f(glGetUniformLocation(shader->program, "material_kd"), 0.5f);
	glUniform1f(glGetUniformLocation(shader->program, "material_ks"), 0.5f);
    glUniform1f(glGetUniformLocation(shader->program, "material_shininess"), 30);


	// texture_offset
    GLint loc_texture_offset = glGetUniformLocation(shader->program, "texture_offset");
	if(offset_texture)
		glUniform2fv(loc_texture_offset, 1, glm::value_ptr(ground_texture_offset));
	else
	    glUniform2f(loc_texture_offset, 0, 0);

	// glUniform3f(glGetUniformLocation(shader->program, "spotlight_direction"), 20);
	glUniform1i(glGetUniformLocation(shader->program, "spotlight_count"), 2*poles_pos.size());
	auto it=poles_pos.begin();
	char temp[100];
	for(int i=0;i<poles_pos.size();i++)
	{
		sprintf(temp, "spotlight_positions[%d]", 2*i);
		glUniform3fv(glGetUniformLocation(shader->program, temp), 1, glm::value_ptr(pole_pos_left(*it)));
		sprintf(temp, "spotlight_positions[%d]", 2*i+1);
		glUniform3fv(glGetUniformLocation(shader->program, temp), 1, glm::value_ptr(pole_pos_right(*it)));
		it++;
	}


	glUniform1i(glGetUniformLocation(shader->program, "lightpoint_count"), gifts_pos.size()+trees_pos.size());
	it=gifts_pos.begin();
	for(int i=0;i<gifts_pos.size();i++)
	{
		sprintf(temp, "lightpoint_positions[%d]", i);
		glUniform3fv(glGetUniformLocation(shader->program, temp), 1, glm::value_ptr(glm::vec3(0,.8f,0)+*it));

		sprintf(temp, "lightpoint_color[%d]", i);
		glUniform3fv(glGetUniformLocation(shader->program, temp), 1, glm::value_ptr(NormalizedRGB(0,0,255)));
		it++;
	}

	// glUniform1i(glGetUniformLocation(shader->program, "lightpoint_count"), gifts_pos.size());
	it=trees_pos.begin();
	glm::vec3 tree_light_offset=glm::rotate(glm::mat4(1), RADIANS(OBJECT_INCLINE), glm::vec3(0, 0, 1))*
		glm::vec4(0, 1.5, 0, 1);

	for(int i=0;i<trees_pos.size();i++)
	{
		sprintf(temp, "lightpoint_positions[%ld]", gifts_pos.size()+i);
		glUniform3fv(glGetUniformLocation(shader->program, temp), 1, glm::value_ptr(tree_light_offset+*it));

		sprintf(temp, "lightpoint_color[%d]", gifts_pos.size()+i);
		glUniform3fv(glGetUniformLocation(shader->program, temp), 1, glm::value_ptr(NormalizedRGB(0,255,0)));

		it++;
	}



    // (student): Do these:
    // - activate texture location 0
    glActiveTexture(GL_TEXTURE0);
    // - bind the texture1 ID
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    // - send theuniform value
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // (student): Generate random texture data
    for(int i=0;i<size;i+=channels)
    {
        data[i+0]=10*((i/3)%width);
        data[i+1]=10*((i/3)/width);
        data[i+2]=100;
    }

    // (student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureID);
    CheckOpenGLError();

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // (student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // (student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            // camera->MoveForward(deltaTime);
            camera->TranslateForward(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(-deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime);
        }

    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
	glm::ivec2 resolution = window->GetResolution();

	glm::vec2 temp=glm::ivec2(mouseX, mouseY)-(resolution/2);
	// std::cout<<temp<<"\n";
	look_angle=((float)temp.x/glm::length(temp));
	look_angle=glm::clamp(look_angle, RADIANS(-45), RADIANS(45));
	// std::cout<<DEGREES(look_angle)<<"\n";
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
	if(button==1 && !mooving)
	{
		reset_game();
	}
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
