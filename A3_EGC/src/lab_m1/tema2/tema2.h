#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema2_camera.h"
#include "lab_m1/tema2/car.h"
#include "lab_m1/tema2/race_map.h"


namespace m1
{


    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void MyRenderMesh(Mesh *mesh, const glm::mat4 &modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void set_main()
        {
            projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, 0.01f, 200.0f);
			drawing_minimap=false;
			current_shader=shaders["MyShader"];
			current_shader->Use();
        }
        void set_minimap()
        {
			drawing_minimap=true;
            float ortho_start=-ortho_width/2;
            float ortho_end=ortho_width/2;
            // std::cout<<ortho_width<<" ";
            // std::cout<<ortho_start<<" ";
            // std::cout<<ortho_end<<"\n";
            // ortho_start=-10;
            // ortho_end=10;
			current_shader=shaders["VertexColor"];
			current_shader->Use();
            projectionMatrix = glm::ortho(ortho_start, ortho_end, ortho_start, ortho_end, 0.01f, 200.0f);
        }

		void Update_positions(float deltaTimeSeconds);
		void draw_all();
		void update_closest_segment();
		void check_offroad();
		void check_colision();

		float dist_from_right(glm::vec4 pos){ return dist_from_right(glm::vec3(pos));}
		float dist_from_right(glm::vec3 pos)
		{
			float distance_from_center=glm::length(pos-race_map.center_points[closest_segment_id]);
			float distance_from_left=glm::length(pos-race_map.left_side_points[closest_segment_id]);
			float distance_from_right=glm::length(pos-race_map.right_side_points[closest_segment_id]);

			return distance_from_left*distance_from_left-3*race_map.segment_width*race_map.segment_width-distance_from_center*distance_from_center;
		}

		float dist_from_left(glm::vec4 pos){ return dist_from_left(glm::vec3(pos));}
		float dist_from_left(glm::vec3 pos)
		{
			float distance_from_center=glm::length(pos-race_map.center_points[closest_segment_id]);
			float distance_from_left=glm::length(pos-race_map.left_side_points[closest_segment_id]);
			float distance_from_right=glm::length(pos-race_map.right_side_points[closest_segment_id]);

			return distance_from_right*distance_from_right-3*race_map.segment_width*race_map.segment_width-distance_from_center*distance_from_center;
		}

     protected:
        implemented::Camera_T2 *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        float FOV;
		bool drawing_minimap=false;
		ViewportArea miniViewportArea;
		Shader *current_shader;


		bool offroad=false;
		Car car;

		tema2::RaceMap race_map;

        float ortho_width=20;

		int closest_segment_id;
    };
}   // namespace m1
