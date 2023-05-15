#ifndef PDF_H
#define PDF_H

#include "GLIncludes.h"
#include <string>

#include <poppler-document.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>
#include <poppler-image.h>

/* 
* PDF class
* this class is used to load the current pdf file,
*   store the current pdf data,
*   and render the current page as raster image
*/
class PDF {
  public:
    PDF() = default;
    ~PDF() = default;

    void loadFile(std::string __filePath);
    bool renderPageImage(int __page);

    // ------- setters -------
    void incrementPage();
    void decrementPage();
    void setCurrentPage(int __page);

    // ------- getters -------
    int getNumPages() const;
    int getCurrentPage() const;
    std::string getFilePath() const;

  private:
    std::string filePath = "";
    poppler::document* doc;
    poppler::page_renderer renderer = poppler::page_renderer();
    poppler::image image;
    int numPages;

    // current page
    int currentPage = -1;

    // temporary image saved for rendering (.jpg or .png)
    std::string img_file_name = "temp.jpg";
};


#endif // PDF_H