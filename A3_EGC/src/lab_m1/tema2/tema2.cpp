#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include "race_map.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera_T2();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	car=Car();
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

	std::cout << "#################" << '\n';
    {
        // Mesh* mesh = new Mesh("plane");
        // mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
		Mesh* mesh = tema2::CreateGrass("plane");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
		Mesh* mesh = tema2::CreateMovingObstacles("obstacle", NormalizedRGB(237, 39, 21));
        // Mesh* mesh = new Mesh("sphere");
        // mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
		Mesh* mesh = tema2::CreateMovingObstacles("car", NormalizedRGB(255, 255, 0));
        // Mesh* mesh = new Mesh("sphere");
        // mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
		std::cout << PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "tracks", "basic.tr") << '\n';
		// race_map=tema2::LoadMap("track", PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "tracks", "snake.tr"), NormalizedRGB(99, 88, 87));
		race_map=tema2::LoadMap("track", PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "tracks", "basic.tr"), NormalizedRGB(99, 88, 87));
		// std::cout<<window->props.selfDir<<"\n";
        meshes[race_map.map_mesh->GetMeshID()] = race_map.map_mesh;
    }

    {
		Mesh* mesh = tema2::CreateTree("tree");
        meshes[mesh->GetMeshID()] = mesh;
    }

	{
		Shader *shader = new Shader("MyShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	// std::cout << race_map.center_points.size() << '\n';
	// std::cout << race_map.segments << '\n';
	// for(int i=race_map.segments-5;i<=race_map.segments;i++)
	// 	std::cout << race_map.center_points[i] << '\n';
	// for(int i=0;i<10;i++)
	// 	std::cout << race_map.center_points[i] << '\n';
	// std::cout << (-5)%50 << '\n';

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    FOV=60;
    ortho_width=100;
    // inPerspective=true;
	closest_segment_id=0;

	glm::ivec2 resolution = window->GetResolution();
	miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

	camera->Set(car.position, car.direction);
	// std::cout << RADIANS(180) << '\n';
}

void Tema2::update_closest_segment()
{
	glm::vec3 car_pos=glm::vec3(car.position);
	float current_dist=glm::length(
		race_map.center_points[closest_segment_id]-
		car_pos
	);
	int last_index=closest_segment_id;
	float next_dist;
	glm::vec3 target;
	for(int direction : {1, -1})
	{
		target=race_map.center_points[(race_map.segments+closest_segment_id+direction)%race_map.segments];
		next_dist=glm::length(target- car_pos);

		// std::cout << next_dist<<"\t"<<next_dist << '\n';

		if(next_dist>current_dist)
			continue;

		do {
			current_dist=next_dist;
			closest_segment_id=(race_map.segments+closest_segment_id+direction)%race_map.segments;

			target=race_map.center_points[(race_map.segments+closest_segment_id+direction)%race_map.segments];
			next_dist=glm::length(target- car_pos);
		} while(next_dist<current_dist);

	}
	// if(last_index!=closest_segment_id)
	// 	std::cout << "next closest_segment_id\t"<<closest_segment_id << '\n';
}


void Tema2::check_colision()
{
	float radius=car.aproximate_radius+race_map.moving_obstacles_radius;
	radius*=sqrt(2);
	// std::cout << radius << '\n';
	for(auto moving_obstacle: race_map.moving_obstacles)
	{
		if(glm::length(glm::vec3(car.position)-moving_obstacle.pos)<radius)
		{
			car.road_state=CarState::moving_obstacle_colision;
			return;
		}
		// else
		// std::cout << glm::length(glm::vec3(car.position)-moving_obstacle.pos) << '\n';
	}
	car.road_state=CarState::normal;
}

void Tema2::check_offroad()
{
	// https://www.desmos.com/calculator/sthlubauue
	// f:dOp^2 -3r^2 - dCent^2
	// f>0 -> outside

	glm::vec3 car_pos=glm::vec3(car.position);
	float distance_from_center=glm::length(car_pos-race_map.center_points[closest_segment_id]);
	float distance_from_left=glm::length(car_pos-race_map.left_side_points[closest_segment_id]);
	float distance_from_right=glm::length(car_pos-race_map.right_side_points[closest_segment_id]);

	float f;
	// std::cout << distance_from_center<<" "<<distance_from_left<<" "<<distance_from_right << '\n';
	if(distance_from_left>distance_from_right)
		// near right
		f=dist_from_right(car_pos);
	else if(distance_from_left<distance_from_right)
		// near left
		f=dist_from_left(car_pos);
	else
		f=-1;

	// std::cout << f << '\n';

	if(f<0)
	{
		offroad=false;
		car.road_state=CarState::normal;
	}
	else if(f<car.aproximate_radius)
	{
		offroad=false;
		car.road_state=CarState::partial_offroad;
	}
	else
	{
		offroad=true;
		car.road_state=CarState::full_offroad;
	}
	// std::cout << offroad << '\n';
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}



void Tema2::Update_positions(float deltaTimeSeconds)
{

	if(car.road_state==CarState::full_offroad)
	{
		glm::vec4 nex_pos=car.position+car.velocity*deltaTimeSeconds;
		bool allowed=false;
		if(dist_from_left(car.position)>0)
			allowed=dist_from_left(nex_pos)<dist_from_left(car.position);
		else
			allowed=dist_from_right(nex_pos)<dist_from_right(car.position);

		if(allowed)
		{
			car.position+=car.velocity*deltaTimeSeconds;
			// std::cout << "MOVED" << '\n';
		}

	}
	else
		car.position+=car.velocity*deltaTimeSeconds;

	float dir_dot=glm::dot(
		normalize(car.velocity),
		normalize(car.direction)
	);
	float direction_velocity_angle=glm::acos(dir_dot);
	// std::cout << normalize(car.velocity)<<" "<<normalize(car.direction) << '\n';
	// std::cout << glm::dot(
	// 	normalize(car.velocity),
	// 	normalize(car.direction)
	// // ) << '\n';
	if(car.road_state==CarState::moving_obstacle_colision)
	{
		// std::cout << glm::length(car.velocity) << '\n';
		car.velocity=glm::vec4(0);
	}
	else
	if(car.road_state==CarState::full_offroad || car.road_state==CarState::moving_obstacle_colision)
	{
		// std::cout << glm::length(car.velocity) << '\n';
		car.velocity*=pow(0.01, deltaTimeSeconds);
		// car.velocity-=car.velocity* sqrt(sqrt(deltaTimeSeconds));
	}
	else if(!isnan(direction_velocity_angle))
	{
		// if(direction_velocity_angle>RADIANS(100))
		// 	direction_velocity_angle=RADIANS(180)-direction_velocity_angle;

		// std::cout << "direction_velocity_angle "<<DEGREES(direction_velocity_angle) << '\n';
		// float corection_angle=min(direction_velocity_angle, RADIANS(45));
		float corection_angle=direction_velocity_angle;
		// car.velocity=glm::proj(car.velocity, car.direction);
		// std::cout << "corection_angle "<<corection_angle << '\n';
		// std::cout << "corection_angle "<<DEGREES(corection_angle) << '\n';
		// std::cout << car.velocity << '\t';
		// if(corection_angle>RADIANS(1))
		// 	car.velocity=car.velocity*cos(corection_angle*deltaTimeSeconds)*glm::rotate(glm::mat4(1), RADIANS(corection_angle*deltaTimeSeconds), glm::vec3(0, 1, 0));
		// std::cout << car.velocity << '\n';

		// std::cout << sin(corection_angle)<<" "<<cos(corection_angle) << '\n';

		glm::vec4 velocity_forward=car.direction*cos(corection_angle)*glm::length(car.velocity);
		// glm::vec4 velocity_perpendicular=rotate(glm::mat4(1), RADIANS(90), glm::vec3(0, 1, 0))*car.direction*sin(corection_angle)*glm::length(car.velocity);
		glm::vec4 velocity_perpendicular=car.velocity-velocity_forward;

		// std::cout << "0\t"<< rotate(glm::mat4(1), RADIANS(90), glm::vec3(0, 1, 0))*car.direction << '\n';
		// std::cout << "1\t"<< rotate(glm::mat4(1), RADIANS(90), glm::vec3(0, 1, 0))*car.direction << '\n';
		// std::cout << "2\t"<< rotate(glm::mat4(1), RADIANS(90), glm::vec3(0, 1, 0))*car.direction*sin(corection_angle) << '\n';
		// std::cout << "3\t"<< rotate(glm::mat4(1), RADIANS(90), glm::vec3(0, 1, 0))*car.direction*sin(corection_angle) *glm::length(car.velocity)<< '\n';



		float velocity_perpendicular_length=glm::length(velocity_perpendicular);
		float velocity_forward_length=glm::length(velocity_forward);
		// std::cout <<"F\t"<<velocity_forward_length << '\n';
		// std::cout <<"\t"<< velocity_forward<<"\n\t"<<velocity_perpendicular <<"\n\t\t"<<velocity_forward+velocity_perpendicular<< '\n';
		// std::cout <<"P\t"<<velocity_perpendicular << '\n';
		// std::cout <<"K\t"<<min(velocity_perpendicular_length, car.perpendicular_friction) << '\n';
		// std::cout <<"K\t"<<min(velocity_perpendicular_length, car.perpendicular_friction)*deltaTimeSeconds << '\n';
		// std::cout <<"N\t"<<normalize(velocity_perpendicular) << '\n';
		glm::vec4 perpendicular_deceleration=glm::vec4(0);
		if(!isnan(normalize(velocity_perpendicular).x))
			perpendicular_deceleration=normalize(velocity_perpendicular)*min(velocity_perpendicular_length, car.perpendicular_friction)*deltaTimeSeconds;
		// std::cout << glm::length(car.velocity) << '\n';
		// std::cout << glm::length(velocity_forward) << '\n';
		// std::cout << glm::length(velocity_perpendicular) << '\n';
		// std::cout << glm::length(perpendicular_deceleration) << '\n';
		// std::cout  << '\n';
		velocity_perpendicular-=perpendicular_deceleration;
		// velocity_forward-=perpendicular_deceleration;
		// std::cout << "velocity_perpendicular_length "<<velocity_perpendicular_length << '\n';
		if(!(window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_S)) && (RADIANS(270)<direction_velocity_angle || direction_velocity_angle<RADIANS(90)))
			velocity_forward-=normalize(velocity_forward)*min(velocity_forward_length, car.forward_friction)*deltaTimeSeconds;


		// std::cout <<"\t"<< velocity_forward<<"\n\t"<<velocity_perpendicular <<"\n\t\t"<<velocity_forward+velocity_perpendicular<< '\n';
		car.velocity=velocity_forward+velocity_perpendicular;
		// car.velocity-=perpendicular_deceleration;

	}
	else
	{
		float velocity_length=glm::length(car.velocity);
		if(velocity_length<=0.001f)
			car.velocity=glm::vec4(0);
		else if(!(window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_S)))
			car.velocity-=normalize(car.velocity)*min(velocity_length, car.forward_friction)*deltaTimeSeconds;

	}

	update_closest_segment();
	check_colision();
	if(car.road_state!=CarState::moving_obstacle_colision)
		check_offroad();

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
		camera->Set(car.position, car.direction);

	for(auto &moving_obstacle: race_map.moving_obstacles)
	{
		// break;
		moving_obstacle.segment_progress+=deltaTimeSeconds*race_map.moving_obstacles_speed;

		float segment_length;
		int  next_index;
		do {
			next_index=(race_map.segments+moving_obstacle.segment_id-1)%race_map.segments;
			segment_length=glm::length(
				race_map.center_points[moving_obstacle.segment_id]-
				race_map.center_points[next_index]
			);
			// std::cout << segment_length << '\n';
			if(moving_obstacle.segment_progress>segment_length){
				moving_obstacle.segment_progress-=segment_length;
				moving_obstacle.segment_id=next_index;
			}
			else
				break;
		} while(true);

		float segment_progress=moving_obstacle.segment_progress/segment_length;


		glm::vec3 progres_left=segment_progress*race_map.left_side_points[next_index]+
								(1-segment_progress)*race_map.left_side_points[moving_obstacle.segment_id];


		glm::vec3 progres_right=segment_progress*race_map.right_side_points[next_index]+
								(1-segment_progress)*race_map.right_side_points[moving_obstacle.segment_id];

		float rescaled_offset=(moving_obstacle.offset+1)/2;
		moving_obstacle.pos=rescaled_offset*progres_right+
								(1-rescaled_offset)*progres_left;

		moving_obstacle.rotation_angle=glm::acos(glm::dot(
			normalize(race_map.center_points[next_index]
				-race_map.center_points[moving_obstacle.segment_id]),
			glm::vec3(1, 0, 0)
		));
		if(isnan(moving_obstacle.rotation_angle))
			moving_obstacle.rotation_angle=0;
	}

}


void Tema2::draw_all()
{
	{
        glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
		// modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, -15));
        MyRenderMesh(meshes["track"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(car.position.x, ROAD_ABOVE_GROUND, car.position.z));
		// modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, car.forward_angle, glm::vec3(0, 1, 0));
		// modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
		// MyRenderMesh(meshes["box"], shaders["VertexColor"], modelMatrix);
        MyRenderMesh(meshes["car"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(car.position.x, 0, car.position.z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 1, 50));
		// modelMatrix = glm::scale(modelMatrix, glm::vec3(100, 1, 100));

		MyRenderMesh(meshes["plane"], modelMatrix);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -10, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(10, 1, 10));
        MyRenderMesh(meshes["plane"], modelMatrix);
    }
    {
		for(auto pos: race_map.tree_positions)
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, 0, pos.z));
			// std::cout << pos << '\n';
			MyRenderMesh(meshes["tree"], modelMatrix);
		}
    }
    {
		for(auto moving_obstacle: race_map.moving_obstacles)
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(moving_obstacle.pos.x, 0, moving_obstacle.pos.z));
			modelMatrix = glm::rotate(modelMatrix, moving_obstacle.rotation_angle, glm::vec3(0, 1, 0));
			MyRenderMesh(meshes["obstacle"], modelMatrix);
		}
    }
}


void Tema2::Update(float deltaTimeSeconds)
{
	Update_positions(deltaTimeSeconds);

	set_main();
    draw_all();

	glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

	set_minimap();
    draw_all();

}


void Tema2::FrameEnd()
{
    // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::MyRenderMesh(Mesh * mesh, const glm::mat4 & modelMatrix)
{
    if (!mesh || !current_shader || !current_shader->program)
        return;

    // Render an object using the specified shader and the specified position
	if(!drawing_minimap)
	    glUniformMatrix4fv(current_shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	else
		glUniformMatrix4fv(current_shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetMinimapViewMatrix()));
    glUniformMatrix4fv(current_shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(current_shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	if(!drawing_minimap)
		glUniform4fv(glGetUniformLocation(current_shader->program, "car_pos"),1,&car.position[0]);


    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
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
	else
	{
		// glm::vec4 nex_pos=car.position+car.velocity*deltaTimeSeconds;
		// if(glm::length(glm::vec3(nex_pos)-race_map.center_points[closest_segment_id])<
		// 		glm::length(glm::vec3(car.position)-race_map.center_points[closest_segment_id]))
		// 	car.position+=car.velocity*deltaTimeSeconds;
		float dir_dot=glm::dot(
			normalize(car.velocity),
			normalize(car.direction)
		);

		float direction_velocity_angle=glm::acos(dir_dot);

		bool is_moving_forward=true;

		if(!isnan(direction_velocity_angle))
			is_moving_forward=direction_velocity_angle<=RADIANS(90) ||
								RADIANS(270)<=direction_velocity_angle;


		if (window->KeyHold(GLFW_KEY_W)) {
			// std::cout << (car.velocity) << '\n';
			bool allowed=true;
			glm::vec4 nex_pos=car.position+car.direction;
			if(car.road_state==CarState::full_offroad)
				if(dist_from_left(car.position)>0)
					allowed=dist_from_left(nex_pos)<dist_from_left(car.position);
				else
					allowed=dist_from_right(nex_pos)<dist_from_right(car.position);

			if(( !is_moving_forward || glm::length(car.velocity)<car.max_speed))
			 	if(allowed)
					car.velocity+=car.direction*car.acceleration*deltaTime;
		}
		else if (window->KeyHold(GLFW_KEY_S)) {
			bool allowed=true;
			glm::vec4 nex_pos=car.position-car.direction;
			if(car.road_state==CarState::full_offroad)
				if(dist_from_left(car.position)>0)
					allowed=dist_from_left(nex_pos)<dist_from_left(car.position);
				else
					allowed=dist_from_right(nex_pos)<dist_from_right(car.position);

			// std::cout << (car.velocity) << '\n';
			if(is_moving_forward || glm::length(car.velocity)<car.max_speed)
				if(allowed)
					car.velocity-=car.direction*car.acceleration*deltaTime;
		}
		// else
		// 	car.velocity=car.velocity*pow(car.deceleration_const, 1/deltaTime);

		float turn_constant=is_moving_forward? 1: -1;

		if (window->KeyHold(GLFW_KEY_A)) {
			float turnning_speed=car.getTurningSpeed();
			car.forward_angle+=turnning_speed*deltaTime*turn_constant;
			car.direction=car.direction*glm::rotate(glm::mat4(1), turnning_speed*deltaTime*turn_constant, glm::vec3(0, -1, 0));
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			float turnning_speed=car.getTurningSpeed();
			car.forward_angle-=turnning_speed*deltaTime*turn_constant;
			car.direction=car.direction*glm::rotate(glm::mat4(1), turnning_speed*deltaTime*turn_constant, glm::vec3(0, 1, 0));
		}
	}



}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    // if (key == GLFW_KEY_T)
    // {
    //     renderCameraTarget = !renderCameraTarget;
    // }
    // // TODO(student): Switch projections
	//
    // if (key == GLFW_KEY_O)
    // {
    //     glm::ivec2 resolution = window->GetResolution();
    //     inPerspective=false;
    //     recalc_ortho();
    // }
    // else
    // if (key == GLFW_KEY_P)
    // {
    //     inPerspective=true;
    //     recalc_perspective();
    // }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        // if (window->GetSpecialKeyState() == 0) {
        //     renderCameraTarget = false;
        //     // (student): Rotate the camera in first-person mode around
        //     // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
        //     // variables for setting up the rotation velocity.
        //     camera->RotateFirstPerson_OX(-sensivityOX*deltaY);
        //     camera->RotateFirstPerson_OY(-sensivityOY*deltaX);
        // }
		//
        // if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
        //     renderCameraTarget = true;
        //     // TODO(student): Rotate the camera in third-person mode around
        //     // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
        //     // variables for setting up the rotation velocity.
        //     camera->RotateThirdPerson_OX(sensivityOX*deltaY);
        //     camera->RotateThirdPerson_OY(sensivityOY*deltaX);
		//
        // }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
