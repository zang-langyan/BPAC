#ifndef PANEL_H
#define PANEL_H

#include "GLIncludes.h"
#include <string>
#include <vector>
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

struct Folder{
  std::string name;
  std::vector<File> files;
  // std::vector<Folder> folders;
};

class ImGui_Panel {
  public:
    ImGui_Panel() = default;
    ~ImGui_Panel() = default;

    void run();
    bool read_PDF_Image(); // initialize image texture
  
  private:
    void menu(); // menu bar
    void bib_export(std::string __save_path); // export bibtex file
    void md_export(std::string __save_path); // export markdown file
    std::filesystem::path save_entry = std::filesystem::current_path().parent_path();
    std::filesystem::path save_outry = std::filesystem::current_path();

  private: // initialize and connect file explorer and pdf reader
    // ImGuiIO* io;
    bool init();

  private: // reader pane
    bool is_reader_open = true;
    void reader(); // pdf reader panel

    // pdf file
    PDF pdf;
    std::string current_filePath = "assets/pdf/BPAC.pdf";
    
    // image texture 
    unsigned char* img; // image data
    GLuint textureID; // texture id
    int width, height, channels; // image size and channels
    int current_page; // current page number

  private: // file manager pane
    bool is_manager_open = true;
    void manager(); // file manager panel

    // folder and file states 
    std::vector<Folder> folders;
    std::vector<File> files; // files in root folder

    // current selected file and folder
    Folder* current_Folder = nullptr;
    File* current_File = nullptr;

  private: // file explorer
    bool is_explorer_open = true;
    void explorer(); // file explorer panel
    poppler::document* pdf_doc; // temporary pdf document
    std::filesystem::path root_entry; // root entry of file explorer


  private: // file information
    bool is_information_open = true;
    void information(); // file information panel
    Information info_getter; // pdf information getter
};

#endif // PANEL_H