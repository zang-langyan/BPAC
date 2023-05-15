#include "Window.h"
#include <ctime>

// Idling for non emscripten, where your app is responsible for the main loop.
// This form of idling will call WaitForEventTimeout(), which may call sleep()
static void IdleBySleeping(FpsIdling& ioIdling)
{
  ioIdling.isIdling = false;
  if ((ioIdling.fpsIdle > 0.f) && ioIdling.enableIdling)
  {
    time_t beforeWait = time(0);
    double waitTimeout = 1. / (double) ioIdling.fpsIdle;

    // Backend specific call that will wait for an event for a maximum duration of waitTimeout
    // (for example glfwWaitEventsTimeout(timeout_seconds))
    glfwWaitEventsTimeout(waitTimeout);

    time_t afterWait = time(0);
    double waitDuration = difftime(afterWait, beforeWait);
    double waitIdleExpected = 1. / ioIdling.fpsIdle;
    ioIdling.isIdling = (waitDuration > waitIdleExpected * 0.9);
  }
}

// -------------------------------------------------------------------

Window::Window(float __width, float __height, std::string __title) {
  width = __width;
  height = __height;
  title = __title;

  // if (!glfwInit())
  //   std::cout << "Error initializing glfw" << std::endl;
  //   exit(1);
  assert(glfwInit() && "Error initializing glfw");

#ifdef __APPLE__
  // GL 3.2 + GLSL 410
  const char* glsl_version = "#version 410";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 410
  const char* glsl_version = "#version 410";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  // Create window with graphics context
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  assert(window);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // ------------------ Initialize OpenGL loader ------------------
  // Initialize GLEW
  if(glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW\n" << std::endl;
  }
  // std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  // std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  // --------------------------------------------------------------

  // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    // io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    // io->ConfigViewportsNoAutoMerge = true;
    //io->ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    // if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      // std::cout << "Viewports enabled" << std::endl;
      style.WindowRounding = 12.0f;
      style.FrameRounding = 10.0f;
      
      // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
      // light style
      style.Colors[ImGuiCol_WindowBg] = ImVec4(0.8f, 1.0f, 1.0f, 0.35f);
      // dark style
      // style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.25f, 0.20f, 0.20f);
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io->Fonts->AddFontDefault();
    //io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io->Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io->Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io->Fonts->AddFontFromFileTTF("assets/fonts/Cousine/Cousine-Regular.ttf", 15.0f);
    io->Fonts->AddFontFromFileTTF("assets/fonts/Noto_Sans_SC/NotoSansSC-Medium.otf", 20.0f, nullptr, io->Fonts->GetGlyphRangesChineseFull());
    //ImFont* font = io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io->Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    show_demo_window = true;
    show_another_window = false;
    // clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // default window background
    // clear_color = ImVec4(0.2f, 0.25f, 0.20f, 0.8f); // dark window background
    clear_color = ImVec4(0.7f, 0.4f, 0.8f, 0.8f); // light window background (0.4, 0.5, 0.9, 1.00)
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

bool Window::run(){
  // ImGui_Panel panels = ImGui_Panel(io);
  // panels.read_PDF_Image(); // initialize welcome page (deprecated)
  // Main loop
  while (!glfwWindowShouldClose(window)) {
#ifndef __EMSCRIPTEN__
    IdleBySleeping(ioState.fpsIdling);
#endif

    glfwPollEvents();
    
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // ------------ Dear ImGui Frame ------------
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    panels.run(clear_color);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // ------------ Dear ImGui frame ------------ END

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
  }

  return true;
}


void Window::stop(){
  glfwSetWindowShouldClose(window, GLFW_TRUE);
}

