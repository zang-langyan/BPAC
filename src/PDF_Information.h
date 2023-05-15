#ifndef PDF_INFORMATION_H
#define PDF_INFORMATION_H

#include "GLIncludes.h"
#include <iostream>
#include <string>

#include <poppler-document.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>
#include <poppler-image.h>

struct File_Info {
  int type;
  std::string title;
  std::string author;
  std::string journal;
  int year;
  std::string publisher;
  std::string notes;

  std::string citekey;
};


/*
* PDF Information class
* this class is used to load pdf meta data
*/
class Information {
  public:
    Information() = default;
    ~Information() = default;

    void loadFile(std::string __filePath);

    // ------- getters -------
    std::string getFilePath();
    std::string getTitle();
    std::string getAuthor();
    std::string getJournal();
    int getYear();
    std::string getPublisher();
    std::string getNotes();
    std::string getCitekey();

    File_Info getFileInfo();

  private:
    std::string filePath = "";
    poppler::document* doc;

    // meta data
    int type = 0;
    std::string title = " ";
    std::string author = " ";
    std::string journal = " ";
    int year = -1;
    std::string publisher = " ";
    std::string notes = " ";
    std::string citekey = " ";
};


#endif