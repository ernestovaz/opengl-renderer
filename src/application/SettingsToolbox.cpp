#include "SettingsToolbox.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../renderer/Settings.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

SettingsToolbox::SettingsToolbox(GLFWwindow* window, Camera* camera) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ItemSpacing = ImVec2(4,8);
    style.IndentSpacing = 100.0f;
    ImVec4 primaryColor = ImVec4(0.40, 0.26, 0.47, 1.0);
    ImVec4 secondaryColor = ImVec4(0.25, 0.21, 0.34, 1.0);
    ImVec4* colors = ImGui::GetStyle().Colors;


    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.43f, 0.42f, 0.47f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.43f, 0.28f, 0.56f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 0.67f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.23f, 0.21f, 0.27f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.23f, 0.21f, 0.27f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.43f, 0.42f, 0.47f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.76f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.58f, 0.44f, 0.61f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.29f, 0.43f, 0.60f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.47f, 0.27f, 0.52f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.43f, 0.42f, 0.47f, 0.54f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.47f, 0.27f, 0.52f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.62f, 0.26f, 0.71f, 0.40f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.50f, 0.31f, 0.50f, 1.00f);

    this->camera = camera;

    ImGui::SetNextWindowCollapsed(true);
}

void SettingsToolbox::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);                     

    ImGui::Text("Framerate: %7.1f FPS", ImGui::GetIO().Framerate);

    if(ImGui::CollapsingHeader("Rendering")){
        ImGui::Indent(10.0f);

        ImGui::PushItemWidth(180);
        ImGui::ColorEdit3("Model Color", glm::value_ptr(Settings::renderingColor));

        ImGui::PushItemWidth(90);
        const char* renderingPrimitives[] = { "Points", "Lines", "Triangles" };
        static int *currentPrimitive = reinterpret_cast<int*>(
                &Settings::renderingPrimitive);
        ImGui::Combo("Rendering Primitive", currentPrimitive, 
                renderingPrimitives, IM_ARRAYSIZE(renderingPrimitives));

        ImGui::Checkbox("Backface Culling", &Settings::cullingEnabled);

        ImGui::PushItemWidth(45);
        static int* currentFaceOrientation = &Settings::reverseFaceOrientation;
        ImGui::Combo("Triangle Face Orientation", currentFaceOrientation, "CW\0CCW\0");

        ImGui::PushItemWidth(60);
        ImGui::DragFloat("Near Plane", &Settings::nearPlane, 0.05);
        ImGui::SameLine();
        ImGui::PushItemWidth(60);
        ImGui::DragFloat("Far Plane", &Settings::farPlane, 0.05);

        ImGui::Unindent(10.0f);
    }

    if(ImGui::CollapsingHeader("Camera")){
        ImGui::Indent(10.0f);
        ImGui::PushItemWidth(150);
        ImGui::SliderFloat("Field Of View", &Settings::fieldOfView, 10.0f, 89.0f);

        if(ImGui::ArrowButton("left", 0)) camera->moveLeft();
        ImGui::SameLine();
        if(ImGui::ArrowButton("front", 2)) camera->moveFront();
        ImGui::SameLine();
        if(ImGui::ArrowButton("back", 3)) camera->moveBack();
        ImGui::SameLine();
        if(ImGui::ArrowButton("right", 1)) camera->moveRight();
        ImGui::SameLine();
        if(ImGui::Button("Up")) camera->moveUp();
        ImGui::SameLine();
        if(ImGui::Button("Down")) camera->moveDown();
        ImGui::SameLine();
        ImGui::Text("Move Camera");

        ImGui::PushItemWidth(100);
        ImGui::InputFloat("Movement Units", &camera->movementUnits, 0.5);

        static bool cameraIsCentered = false;
        if(ImGui::Checkbox("Center Camera", &cameraIsCentered)) {
            camera->center();
        }
        if(ImGui::Button("Reset Camera Position")) {
            camera->reset();
        }

        ImGui::Unindent(10.0f);
    }

    if (ImGui::Button("Quit")) quitButtonClicked = true;

    ImGui::End();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

SettingsToolbox::~SettingsToolbox() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool SettingsToolbox::isHandlingMouse() {
    return ImGui::GetIO().WantCaptureMouse;
}

