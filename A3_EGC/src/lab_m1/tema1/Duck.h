#pragma once

#include "components/simple_scene.h"
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include <unordered_map>
#include <list>
#include "timer.h"

#define Duck_draw(duck_instance) \
do { \
	for(auto element_id:duck_instance.render_elements) \
		RenderMesh2D(duck_instance.meshes[element_id], shaders["VertexColor"], duck_instance.baseMatrix*duck_instance.translates[element_id]); \
} while(false);

#define DUCK_ESCAPE_AFTER 5
#define DUCK_BODY_LIFE 1

namespace m1
{
	enum DuckStates
	{
		free_flying,
			shot_down,
				crashed,
			escaping,
				escaped,
	};

    class Duck
    {
    public:
        static std::unordered_map<std::string, Mesh*> meshes;
        static std::unordered_map<std::string, glm::mat3> translates;
		static std::list<std::string> render_elements;
        static int test;
        static void InitMesh();

    public:
        void setPosition(float x, float y);
        void setScale(float x, float y);
        void setScale(float scale);
        void setRotation(float angle);
		void recalcBaseMatrix();
		void tick(float deltaTime);
		void tickWing(float deltaTime);
        void tickFreeFly(float deltaTime);
        void draw(gfxc::SimpleScene scene);
		// from -1 to 1
		void setFlapAngle(float state);
		bool clickedOn(float ClickX, float ClickY);
		void shot();
		void escape();

        Duck();
        ~Duck();

        glm::mat3 baseMatrix;
		glm::vec3 forcedMove;
        float x, y, scaleX, scaleY;
		float rotation_angle;

		static float base_fly_speed;
        float fly_speed;
		static float coalisionX[2], coalisionY[2];

		float wingFlapSpeed;
		// used for direction
		bool wingFlapState;
		float wingFlapAngle;

		bool onScreenColision;
		DuckStates state;

		timer::Timer escapeTimer;
		bool escapeing=false;

		timer::Timer crashTimer;
		bool crashed=false;
    };
}
