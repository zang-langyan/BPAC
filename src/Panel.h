#ifndef PANEL_H
#define PANEL_H

#include "GLIncludes.h"
#include <string>
// #include <vector>
// #include <list>
#include <set>
// #include <map>
#include <iostream>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "PDF.h"
#include "PDF_Information.h"

#include <fstream>
#include <filesystem>


struct File{
  std::string name;
  std::string path;
  File_Info info;
};

struct File_Comp{
  bool operator()(const File& lhs, const File& rhs) const { 
    return lhs.path < rhs.path; 
  }
};

struct Folder{
  std::string name;
  std::set<File,File_Comp> files;
};

struct Folder_Comp{
  bool operator()(const Folder& lhs, const Folder& rhs) const { 
    return lhs.name < rhs.name; 
  }
};

struct State{
  std::set<File,File_Comp> files;
  std::set<Folder,Folder_Comp> folders;
  Folder current_Folder;
  File current_Folder_File;
  File current_Root_File;
};

class ImGui_Panel {
  public:
    ImGui_Panel() = default;
    ~ImGui_Panel() = default;

    void run(ImVec4& __clear_color);
    bool read_PDF_Image(); // initialize image texture
  
  private:
    void menu(ImVec4& __clear_color); // menu bar
    void bib_export(std::string __save_path); // export bibtex file
    void md_export(std::string __save_path); // export markdown file
    std::filesystem::path save_entry = std::filesystem::current_path().parent_path();
    std::filesystem::path save_outry = std::filesystem::current_path();

  private: // initialize and connect file explorer and pdf reader
    // ImGuiIO* io;
    bool init();

  private: // reader pane
    void reader(); // pdf reader panel

    // pdf file
    // std::string current_filePath = "assets/pdf/BPAC_video.pdf";
    
    // image texture 
    unsigned char* img; // image data
    GLuint textureID; // texture id
    int width, height, channels; // image size and channels
    int current_page; // current page number

  private: // file manager pane
    void manager(); // file manager panel

    // folder and file states 
    // std::map<std::string, Folder> folders;
    // std::map<std::string, File> files; // files in root folder
    // std::set<Folder> folders;
    // std::set<File> files; // files in root folder

    // current selected file and folder
    // std::string current_folder = "";
    // std::string current_file = "";
    // Folder* current_Folder = nullptr;
    // File* current_File = nullptr;

  private: // file explorer
    void explorer(); // file explorer panel


  private: // file information
    void information(); // file information panel
};

#endif // PANEL_H