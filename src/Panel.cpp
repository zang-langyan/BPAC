#include "Panel.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// -------------------------------------------------------------------------------

void ImGui_Panel::run(ImVec4& __clear_color) {
  menu(__clear_color);
  if (is_explorer_open)
    explorer();

  if (is_manager_open)
    manager();

  init();
  if (is_reader_open)
    reader();

  if (is_information_open)
    information();

}

// -------------------------------------------------------------------------------
// menu bar
void ImGui_Panel::bib_export(std::string __save_path){
  std::ofstream bib_file(__save_path);

  // write header
  bib_file << "%% Document created by BPAC++ (by 臧朗彦)\n\n";

  for (auto& file : files) {
    bib_file << "@article{" << file.info.citekey << ",\n";
    bib_file << "  title={" << file.info.title << "},\n";
    bib_file << "  author={" << file.info.author << "},\n";
    bib_file << "  journal={" << file.info.journal << "},\n";
    bib_file << "  year={" << file.info.year << "},\n";
    bib_file << "  publisher={" << file.info.publisher << "},\n";
    bib_file << "  notes={" << file.info.notes << "}\n";
    bib_file << "}\n\n";
  }

  for (auto& folder : folders) {
    for (auto& file : folder.files) {
      bib_file << "@article{" << file.info.citekey << ",\n";
      bib_file << "  title={" << file.info.title << "},\n";
      bib_file << "  author={" << file.info.author << "},\n";
      bib_file << "  journal={" << file.info.journal << "},\n";
      bib_file << "  year={" << file.info.year << "},\n";
      bib_file << "  publisher={" << file.info.publisher << "},\n";
      bib_file << "  notes={" << file.info.notes << "}\n";
      bib_file << "}\n\n";
    }
  }

  bib_file.close();
}

void ImGui_Panel::md_export(std::string __save_path){
  std::ofstream md_file(__save_path);

  // write header
  md_file << "<!-- Document created by BPAC++ (by 臧朗彦) -->\n\n";
  md_file << "## Bibliography Summary\n\n";
  md_file << "| Title | Author | Journal | Year | Publisher | Notes |\n";

  for (auto& file: files) {
    // md_file << "| " << "<a href= \"" << file.path << "\"> " <<  file.info.title << "</a>"
    md_file << "| " << file.info.title
            << " | " << file.info.author 
            << " | " << file.info.journal 
            << " | " << file.info.year 
            << " | " << file.info.publisher 
            << " | " << file.info.notes 
            << " |\n";
  }

  for (auto& folder : folders) {
    for (auto& file : folder.files) {
      // md_file << "| " << "<a href= \"" << file.path << "\"> " <<  file.info.title << "</a>"
      md_file << "| " << file.info.title
              << " | " << file.info.author 
              << " | " << file.info.journal 
              << " | " << file.info.year 
              << " | " << file.info.publisher 
              << " | " << file.info.notes 
              << " |\n";
    }
  }

  md_file.close();
}

void ImGui_Panel::menu(ImVec4& __clear_color){

  ImGui::BeginMainMenuBar();

  static bool bib_popup_open = false;
  static bool md_popup_open = false;
  if(ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Open")) {
      // to be implemented
    }
    if (ImGui::MenuItem("Save")) {
      // to be implemented
    }
    if (ImGui::MenuItem("Export .bib")){
      bib_popup_open = true;
    }

    if (ImGui::MenuItem("Export .md")){
      md_popup_open = true;
    }

    ImGui::EndMenu();
  }


  if (ImGui::BeginMenu("Settings")) {
    if (ImGui::BeginMenu("Panels")){
      ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
      ImGui::Checkbox("PDF reader", &is_reader_open);
      ImGui::PopItemFlag();

      ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
        ImGui::Checkbox("File manager", &is_manager_open);
      ImGui::PopItemFlag();

      ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
        ImGui::Checkbox("File explorer", &is_explorer_open);
      ImGui::PopItemFlag();

      ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
        ImGui::Checkbox("PDF information", &is_information_open);
      ImGui::PopItemFlag();

      ImGui::EndMenu();
    }
   
    if (ImGui::BeginMenu("Preference")){
      ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
        ImGui::ColorEdit3("Background Color", (float*)&__clear_color);
      ImGui::PopItemFlag();

      if (ImGui::BeginMenu("Style")){
        if (ImGui::MenuItem("Dark")) {
          ImGui::StyleColorsDark();
        }
        if (ImGui::MenuItem("Light")) {
          ImGui::StyleColorsLight();
        }
        if (ImGui::MenuItem("Classic")) {
          ImGui::StyleColorsClassic();
        }
        ImGui::EndMenu();
      }

      ImGui::EndMenu();
    }

    ImGui::EndMenu();
  }


  // -------------------------------------------------------
  // ------------------ Export .bib popup ------------------
  // -------------------------------------------------------
  if (bib_popup_open) {
    ImGui::OpenPopup("##folder_chooser_bib");
  }

  ImGui::SetNextWindowBgAlpha(0.75f);
  if (ImGui::BeginPopupModal("##folder_chooser_bib")) {

    if (ImGui::ArrowButton("##up_level_save_bib", ImGuiDir_Up)) {
      save_entry = save_entry.parent_path();
    }
    ImGui::Separator();

    // File Tree
    static int save_selected = -1;
    static int save_id = 0;
    for (auto& entry : std::filesystem::directory_iterator(save_entry)) {
      char folderName[256];
      if (entry.is_directory()) {
        sprintf(folderName, "%s",entry.path().filename().string().c_str());
        if (ImGui::Selectable(folderName, save_selected == save_id)) {
          save_selected = save_id;
          save_outry = entry.path();
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
          save_entry = entry.path();
        }
        save_id++;
      }
      
    }

    ImGui::Separator();

    // save file name
    static char save_file_name[128];
    ImGui::InputTextWithHint("##save_file_name_bib", "Enter file name to be saved", save_file_name, IM_ARRAYSIZE(save_file_name));
    ImGui::SameLine();
    if (ImGui::Button("Confirm##save_bib")) {
      std::string save_path = save_outry.string() + "/" + save_file_name + ".bib";
      std::cout << save_file_name << std::endl;
      std::cout << save_path << std::endl;
      bib_export(save_path);
      ImGui::CloseCurrentPopup();
      bib_popup_open = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel##save_bib")) {
      ImGui::CloseCurrentPopup();
      bib_popup_open = false;
    }

    ImGui::EndPopup();
  }
  // -------------------------------------------------------
  // ------------------ Export .bib popup ------------------ END
  // -------------------------------------------------------

  // -------------------------------------------------------
  // ------------------ Export .md popup -------------------
  // -------------------------------------------------------
  if (md_popup_open) {
    ImGui::OpenPopup("##folder_chooser_md");
  }

  ImGui::SetNextWindowBgAlpha(0.75f);
  if (ImGui::BeginPopupModal("##folder_chooser_md")) {

    if (ImGui::ArrowButton("##up_level_save_md", ImGuiDir_Up)) {
      save_entry = save_entry.parent_path();
    }
    ImGui::Separator();

    // File Tree
    static int save_selected = -1;
    static int save_id = 0;
    for (auto& entry : std::filesystem::directory_iterator(save_entry)) {
      char folderName[256];
      if (entry.is_directory()) {
        sprintf(folderName, "%s",entry.path().filename().string().c_str());
        if (ImGui::Selectable(folderName, save_selected == save_id)) {
          save_selected = save_id;
          save_outry = entry.path();
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
          save_entry = entry.path();
        }
        save_id++;
      }
      
    }

    ImGui::Separator();

    // save file name
    static char save_file_name[128];
    ImGui::InputTextWithHint("##save_file_name_md", "Enter file name to be saved", save_file_name, IM_ARRAYSIZE(save_file_name));
    ImGui::SameLine();
    if (ImGui::Button("Confirm##save_md")) {
      std::string save_path = save_outry.string() + "/" + save_file_name + ".md";
      std::cout << save_file_name << std::endl;
      std::cout << save_path << std::endl;
      md_export(save_path);
      ImGui::CloseCurrentPopup();
      md_popup_open = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel##save_md")) {
      ImGui::CloseCurrentPopup();
      md_popup_open = false;
    }

    ImGui::EndPopup();
  }
  // -------------------------------------------------------
  // ------------------ Export .md popup ------------------- END
  // -------------------------------------------------------


  ImGui::EndMainMenuBar();

}

// -------------------------------------------------------------------------------
// initialize and connect file explorer and pdf reader

bool ImGui_Panel::init() {
  if (pdf.getFilePath() != current_filePath) {
    pdf.loadFile(current_filePath);
    current_page = 1;
  }
  return true;
}

bool ImGui_Panel::read_PDF_Image() {
  // load image
  // std::ifstream f("temp.jpg");
  // if (f.good()) {
    img = stbi_load("temp.jpg", &width, &height, NULL, 4);
  // } else {
  //   img = stbi_load("welcome.jpg", &width, &height, NULL, 4);
  // }
  // f.close();
  
  if (!img) {
    std::cerr << "Could not load image" << std::endl;
    exit(1);
  }
  
  // create texture
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  // float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  // set texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minification filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnification filter

  // generate texture
  glTexImage2D(
    GL_TEXTURE_2D, // target
    0, // level
    GL_RGBA, // internal format
    width, // width
    height, // height
    0, // border
    GL_RGBA, // format
    GL_UNSIGNED_BYTE, // type
    img // data
  );

  stbi_image_free(img);

  return true;
}

void ImGui_Panel::reader() {
  // render page image
  if (pdf.getCurrentPage() != current_page-1 &&
      current_page >= 1 && 
      current_page <= pdf.getNumPages()
  ) 
  {
    pdf.renderPageImage(current_page-1);
    pdf.setCurrentPage(current_page-1);
    // read image when page changes
    read_PDF_Image();
  }

  // ------------------ imgui pane ------------------
  ImGui::SetNextWindowSize(ImVec2(695,1075), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(605, 26), ImGuiCond_FirstUseEver);
  ImGui::Begin("PDF Reader", &is_reader_open);
  float ratio = (float)width / (float)height; // retain the page scale
  ImGui::Image((void*)(intptr_t)textureID, ratio > 1. ? ImVec2(960, 960/ratio) : ImVec2(960*ratio, 960));
  
  if (ImGui::ArrowButton("##previous_page", ImGuiDir_Left) && current_page > 0) {
    current_page--;
  }
  ImGui::SameLine();
  ImGui::Text("Page %d / %d", current_page, pdf.getNumPages());
  ImGui::SameLine();
  if (ImGui::ArrowButton("##next_page", ImGuiDir_Right) && current_page <= pdf.getNumPages()) {
    current_page++;
  }

  ImGui::Text("Jump to page: ");
  ImGui::SameLine();
  ImGui::InputInt("##current_page_input", &current_page, 1, pdf.getNumPages()); 
  ImGui::SameLine();
  ImGui::Text("/%d", pdf.getNumPages());

  ImGui::End();
  // -------------------------------------------------
}

// -------------------------------------------------------------------------------
// file manager pane
void ImGui_Panel::manager(){
  ImGui::SetNextWindowSize(ImVec2(550,548), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(2, 598), ImGuiCond_FirstUseEver);
  ImGui::Begin("File Manager", &is_manager_open);

    // current_Folder = nullptr;
    // current_File = nullptr;
    // add folder
    static char newFolderName[256] = "";
    ImGui::PushItemWidth(156.f);
    ImGui::InputTextWithHint("##Add new folder", "Enter new folder name", newFolderName, IM_ARRAYSIZE(newFolderName));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Add Folder")) {
      Folder folder;
      folder.name = newFolderName;
      folders.push_back(folder);
    }

    // delete button
    ImGui::SameLine();
    if (ImGui::Button("Delete Folder")) {
      if (current_Folder != nullptr) {
        folders.erase(folders.begin()+(current_Folder-folders.data()));
      } 
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete File")) {
      if (current_File != nullptr && current_Folder != nullptr) {
        current_Folder->files.erase(current_Folder->files.begin() + (current_File-current_Folder->files.data()));
      } else if (current_File != nullptr) {
        files.erase(files.begin() + (current_File-files.data()));
      }
    }
      
    ImGui::Separator();

    // File Tree
    // Root
    static int root_file_selected = -1;
    static int folder_file_selected = -1;
    for (int fileID = 0; fileID < files.size(); fileID++) {
      char fileName[256];
      sprintf(fileName, "%s##%d_root_file",files[fileID].name.c_str(), fileID);

      if (ImGui::Selectable(fileName, root_file_selected == fileID)){
        root_file_selected = fileID;
        folder_file_selected = -1;
        current_filePath = files[fileID].path;
        current_File = &files[fileID];
      }

    }

    // Folders
    int folder_selected = -1;
    for (int folderID = 0; folderID < folders.size(); folderID++) {
      ImGui::Separator();

      char folderName[256];
      sprintf(folderName, "%s##%d_folder",folders[folderID].name.c_str(), folderID);
      if (ImGui::CollapsingHeader(folderName)){ // folder
        folder_selected = folderID;
        current_Folder = &folders[folderID];

        for (int fileID = 0; fileID < folders[folderID].files.size(); fileID++) {
          char fileName[256];
          sprintf(fileName, "%s##%d%d_folder_file",folders[folderID].files[fileID].name.c_str(), folderID, fileID);

          if (ImGui::Selectable(fileName, folder_file_selected == fileID)){
            folder_file_selected = fileID;
            root_file_selected = -1;
            current_filePath = folders[folderID].files[fileID].path;
            current_File = &folders[folderID].files[fileID];
          }

        }

      }

    }
    
    // make sure root and folders can be distinguished
    if (folder_selected == -1) {
      current_Folder = nullptr;
    }

  ImGui::End();
  // -------------------------------------------------
}

// -------------------------------------------------------------------------------
// ------------------ File explorer ------------------
void ImGui_Panel::explorer()
{
  ImGui::SetNextWindowSize(ImVec2(550,571), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(1, 26), ImGuiCond_FirstUseEver);
  ImGui::Begin("File Explorer", &is_explorer_open, ImGuiWindowFlags_MenuBar );
  // root entry of file explorer
  if (root_entry.empty()) {
    // root_entry = std::filesystem::current_path().root_path();
    root_entry = std::filesystem::current_path().parent_path();
  }


  // Menu Bar for the file system
  if (ImGui::BeginMenuBar()) {
    if (ImGui::Button("Previous")) {
      root_entry = root_entry.parent_path();
    }
    ImGui::EndMenuBar();
  }

  ImGui::Separator();

  // File Tree
  for (auto& entry : std::filesystem::directory_iterator(root_entry)) {
    char fileName[256];
    sprintf(fileName, "%s",entry.path().filename().string().c_str());
    if (ImGui::Selectable(fileName)) {
      if (entry.is_directory()) {
        root_entry = entry.path();
      } else if (entry.path().extension() == ".pdf") {
        
        info_getter.loadFile(entry.path().string());

        if (current_Folder == nullptr)
        {

          files.push_back(
            File{
              entry.path().filename().string(), // name
              entry.path().string(), // path
              info_getter.getFileInfo(), // pdf information
            }
          );

        } else {
          current_Folder->files.push_back(
            File{
              entry.path().filename().string(), // name
              entry.path().string(), // path
              info_getter.getFileInfo(), // pdf information
            }
          );
        }

      } else {
        std::cout << "file not supported" << std::endl;
      }
    }

  }

  ImGui::End();
}

// ------------------ File explorer ------------------ END


// -------------------------------------------------------------------------------
// --------------- PDF information panel ------------------
void ImGui_Panel::information(){
  ImGui::SetNextWindowSize(ImVec2(395,816), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(1380, 50), ImGuiCond_FirstUseEver);
  ImGui::Begin("PDF Cite Information", &is_information_open);

    // Type
    const char* pdf_type[] = {"article", "book", "booklet", "inbook", "inproceedings/conference", "incollection", "manual", "phdthesis/mastersthesis", "techreport", "misc", "proceedings", "unpublished"}; 
    static int type_holder = 0;
    // ImGui::Combo("Type", current_File != nullptr? &current_File->info.type : &type_holder, pdf_type, IM_ARRAYSIZE(pdf_type));
    if (ImGui::BeginCombo("Type", pdf_type[current_File != nullptr? current_File->info.type : type_holder])) {
      for (int n = 0; n < IM_ARRAYSIZE(pdf_type); n++) {
        bool is_selected = (current_File != nullptr? current_File->info.type : type_holder) == n;
        if (ImGui::Selectable(pdf_type[n], is_selected)) {
          if (current_File != nullptr) {
            current_File->info.type = n;
          } else {
            type_holder = n;
          }
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }


    // Title
    ImGui::Separator();
    // text
    static char title_text[2048] = "BPAC++";
    ImGui::Text("Title: %s", current_File != nullptr? current_File->info.title.c_str() : title_text);
    // input
    static char title_temp[2048] = "\0";
    ImGui::PushItemWidth(156.f);
      ImGui::InputTextWithHint("##change_title", "change title", title_temp, IM_ARRAYSIZE(title_temp));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Confirm##title") && current_File != nullptr) {
      current_File->info.title = title_temp;
    }


    // Author
    ImGui::Separator();
    // text
    static char author_text[512] = u8"臧朗彦";
    ImGui::Text("Author: %s", current_File != nullptr? current_File->info.author.c_str() : author_text);
    // input
    static char author_temp[512] = "\0";
    ImGui::PushItemWidth(156.f);
      ImGui::InputTextWithHint("##change_author", "change author", author_temp, IM_ARRAYSIZE(author_temp));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Confirm##author") && current_File != nullptr) {
      current_File->info.author = author_temp;
    }


    // Journal
    ImGui::Separator();
    // text
    static char journal_text[512] = "Journal of Bibliography Preview And Citation";
    ImGui::Text("Journal: %s", current_File != nullptr? current_File->info.journal.c_str() : journal_text);
    // input
    static char journal_temp[512] = "\0";
    ImGui::PushItemWidth(156.f);
      ImGui::InputTextWithHint("##change_journal", "change journal", journal_temp, IM_ARRAYSIZE(journal_temp));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Confirm##journal") && current_File != nullptr) {
      current_File->info.journal = journal_temp;
    }

    
    // Year
    ImGui::Separator();
    static int year_holder = 2023;
    ImGui::InputInt("Year", current_File != nullptr? &current_File->info.year : &year_holder);


    // Publisher
    ImGui::Separator();
    // text
    static char publisher_text[512] = "Bibliography Preview And Citation(BPAC) Publisher";
    ImGui::Text("Publisher: %s", current_File != nullptr? current_File->info.publisher.c_str() : publisher_text);
    // input
    static char publisher_temp[512] = "\0";
    ImGui::PushItemWidth(156.f);
      ImGui::InputTextWithHint("##change_publisher", "change publisher", publisher_temp, IM_ARRAYSIZE(publisher_temp));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Confirm##publisher") && current_File != nullptr) {
      current_File->info.publisher = publisher_temp;
    }


    // Notes
    ImGui::Separator();
    static char notes_temp[2048];
    strcpy(notes_temp, current_File != nullptr? current_File->info.notes.c_str() : "");
    ImGui::Text("Notes: ");
    ImGui::InputTextMultiline("##notes_input", notes_temp, IM_ARRAYSIZE(notes_temp));
    ImGui::SameLine();
    if (ImGui::Button("Confirm##notes") && current_File != nullptr) {
      current_File->info.notes = notes_temp;
    }


    // Citation Key
    ImGui::Separator();
    // text
    static char citekey_text[512] = "BPAC-Zang-2023";
    ImGui::Text("Citation Key: %s", current_File != nullptr? current_File->info.citekey.c_str() : citekey_text);
    // input
    static char cite_key_temp[512] = "\0";
    ImGui::PushItemWidth(156.f);
      ImGui::InputTextWithHint("##change_cite_key", "change cite key", cite_key_temp, IM_ARRAYSIZE(cite_key_temp));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Confirm##cite_key") && current_File != nullptr) {
      current_File->info.citekey = cite_key_temp;
    }

  ImGui::End();
}
// --------------- PDF information panel ------------------ END