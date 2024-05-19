#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "components/text_renderer.h"


#include "lab_m1/tema3/tema3_camera.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
     public:
        Tema3();
        ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture, bool offset_texture=false);
        Texture2D *CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

		void draw();
		void drawPlayer(glm::vec3 pos, float look_angle);
		void drawTree(glm::vec3 pos);
		void drawGift(glm::vec3 pos);
		void drawRock(glm::vec3 pos);
		void drawPole(glm::vec3 pos);

		void reset_game();

		gfxc::TextRenderer *textRenderer;

        std::unordered_map<std::string, Texture2D *> mapTextures;

		implemented::Camera_T3 *camera;
		glm::mat4 projectionMatrix;

		bool mooving;
		float look_angle;
		int score;

		glm::vec2 ground_texture_offset;

		std::list<glm::vec3> trees_pos;
		std::list<glm::vec3> gifts_pos;
		std::list<glm::vec3> rocks_pos;
		std::list<glm::vec3> poles_pos;

		// std::list<glm::vec3> spotlights_pos;
		// std::list<glm::vec3> spotlights_pos;

    };
}   // namespace m1
