#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"


namespace m1
{
    class Lab5 : public gfxc::SimpleScene
    {
     public:
        Lab5();
        ~Lab5();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void recalc_perspective()
        {
            projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, 0.01f, 200.0f);

        }
        void recalc_ortho()
        {
            float ortho_start=-ortho_width/2;
            float ortho_end=ortho_width/2;
            std::cout<<ortho_width<<" ";
            std::cout<<ortho_start<<" ";
            std::cout<<ortho_end<<"\n";
            // ortho_start=-10;
            // ortho_end=10;
            projectionMatrix = glm::ortho(-10.f, 10.f, ortho_start, ortho_end, 0.01f, 200.0f);
        }
     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        float FOV;
        bool inPerspective=true;

        float ortho_width=20;
    };
}   // namespace m1
