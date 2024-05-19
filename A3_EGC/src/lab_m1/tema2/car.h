#pragma once

#include "components/simple_scene.h"
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include <unordered_map>
#include <list>
#include <iostream>
// #include "timer.h"

//
// #define Duck_draw(duck_instance) \
// do { \
// 	for(auto element_id:duck_instance.render_elements) \
// 		RenderMesh2D(duck_instance.meshes[element_id], shaders["VertexColor"], duck_instance.baseMatrix*duck_instance.translates[element_id]); \
// } while(false);


namespace m1
{
	enum CarState {
		normal,
		partial_offroad,
		full_offroad,
		moving_obstacle_colision,
	};

    class Car
    {
	public:

        Car();
        ~Car();

		float getTurningSpeed()
		{
			// if(glm::length(velocity)<min_speed_4_truning)
			// 	return 0;


			float length=glm::length(velocity);
			if(length<min_speed_4_truning)
				return 0;
			float factor=(length-min_speed_4_truning)/(max_speed-min_speed_4_truning+1);
			factor-=factor*factor;
			// std::cout << "factor "<<factor << '\n';
			float turn_speed=min_turining_speed*factor+(1-factor)*max_turining_speed;
			// std::cout << "turn_speed "<<turn_speed << '\n';
			// turn_speed*=(glm::length(velocity)-min_speed_4_truning)/(max_speed-min_speed_4_truning);
			return turn_speed;
		}

        glm::mat4 baseMatrix;
		glm::vec4 direction, velocity, position;

		float rotation_angle;
		float max_speed;
		float forward_angle;

		float acceleration;
		float min_turining_speed, max_turining_speed;
		float min_speed_4_truning;
		float deceleration_const=0.9;

		float perpendicular_friction;
		float forward_friction;

		float aproximate_radius=0.5;

		CarState road_state=CarState::normal;
    };
}
