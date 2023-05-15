#ifndef WINDOW_H
#define WINDOW_H

#include "GLIncludes.h"

#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <fstream>
#include "Panel.h"

struct FpsIdling
{
  float fpsIdle = 9.f;         // FPS when idling
  bool  enableIdling = true;   // a bool to enable/disable idling
  bool  isIdling = false;      // an output parameter filled by the runner
};

// This is your application/runner state.
// Call it whatever you want, store whatever you want in it!
struct RunnerState
{
  // [...] Lots of other params, probably
  FpsIdling fpsIdling;
};

class Window {
  public:
    Window(float __width, float __height, std::string __title);
    virtual ~Window();

    bool run();

  protected:
    void stop();

    // virtual bool init() = 0;
    // virtual bool render() = 0;
    // virtual bool update() = 0;
    // virtual bool end() = 0;

    GLFWwindow* window = nullptr;
    float width;
    float height;
    std::string title;

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  private:
    ImGuiIO* io; 

    ImGui_Panel panels;

    RunnerState ioState;
};



#endif // WINDOW_H